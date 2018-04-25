#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>

#include <iostream>
#include <string>
#include <vector>

#include "clamav_engine.h"

#include "clock_time.h"

#include "gperftools/profiler.h"

static ClamavEngine* clamav = nullptr;
static std::string av_database;
static std::string scan_path;


static ssize_t get_file_size(const char* path)
{
    ssize_t file_size = -1;
    struct stat statbuff;

    if (stat(path, &statbuff) < 0) {
        return file_size;
    } else {
        file_size = statbuff.st_size;
        return file_size;
    }
}

int buildGlobalEngine(const char* path)
{
    clamav = new ClamavEngine(path);
    if (nullptr == clamav) {
        return -1;
    }
    clamav->buildEngine();
}

void destroyGlobalEngine()
{
    delete clamav;
}



int get_files(const char* path, std::vector<std::string>& files)
{
    std::map<std::string, DIR*> dir_maps;

    struct stat st;
    if (stat(path, &st) == -1) {
        fprintf(stderr, "stat : %s \n", strerror(errno));
        return -1;
    }

    if (S_ISREG(st.st_mode)) {
        files.push_back(std::string(path));
        return 1;
    } else if (S_ISDIR(st.st_mode)) {
        dir_maps[std::string(path)] = NULL;
    }

    int num = 0;
    std::vector<std::string> sub_dirs;

    do {
        sub_dirs.clear();
        std::map<std::string, DIR*>::iterator it;
        for (it = dir_maps.begin(); it != dir_maps.end(); it++) {
            it->second = opendir(it->first.c_str());
            if (it->second == NULL) {
                fprintf(stderr, "opendir %s : %s \n", it->first.c_str(), strerror(errno));
                continue;
            }

            struct dirent* sub_dir = NULL;
            while ((sub_dir = readdir(it->second)) != NULL) {
                std::string sub_name = std::string(sub_dir->d_name);
                if (sub_name == "." || sub_name == "..")
                {
                    continue;
                }
                std::string full_path = it->first + "/" + sub_name;
                if (sub_dir->d_type == DT_REG) {
                    files.push_back(full_path);
                    num++;
                } else if (sub_dir->d_type == DT_DIR) {
                    sub_dirs.push_back(full_path);
                }
                //printf("%s \n", full_path.c_str());
            }
            closedir(it->second);
        }

        dir_maps.clear();
        for (std::vector<std::string>::iterator it = sub_dirs.begin(); it != sub_dirs.end(); it++) {
            dir_maps[*it] = NULL;
        }

    } while(sub_dirs.size() > 0);

    return num;
}


int scan_file(const char* file_name, int if_fd)
{
    ssize_t file_size = get_file_size(file_name);
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL) {
        fprintf(stderr, "open error  %s \n", strerror(errno));
        return -1;
    }
    ClamavScanResult result;
    int ret = 0;

    if (if_fd) {
        clamav->scanFileFdStd(fileno(fp), &result);
    } else {
        char* file_mem = new char[file_size];
        fread(file_mem, 1, file_size, fp);
        ret = clamav->ScanFmap(file_mem, file_size, &result, CL_SCAN_STDOPT);
        delete[] file_mem;
    }

    printf("name=%s \n"
                    "\tsize=%ld,"
                    "ret=%d , "
                    "stat=%d , "
                    "type[%s], virname[%s] \n"
                    , file_name
                    , file_size
                    , ret
                    , result.stat
                    , result.type
                    , result.virname);
    fclose(fp);
}


int main(int arc, char* argv[]) {

    int opt = 0;
    int if_fd = 0;

    while (opt =  getopt(arc, argv, "d:s:f"))
    {
        if (opt == -1) {
            break;
        }

        switch (opt) {
            case 'd':
                av_database = std::string(optarg);
                break;
            case 's':
                scan_path = std::string(optarg);
                printf("%s \n", scan_path.c_str());
                break;
            case 'f':
                if_fd = 1;
                break;
            default:
                exit(-1);
                break;
        }
    }

    std::vector<std::string> files;
    get_files(scan_path.c_str(), files);

    printf("clamav version=%s \n", clamav->getClamavVersion().c_str());
    buildGlobalEngine(av_database.c_str());

     ProfilerStart("./scan.perf");
     ClockTime clock_time;
     clock_time.GatherNow();

    for (std::vector<std::string>::iterator it = files.begin();
         it != files.end(); it++) {
        scan_file(it->c_str(), if_fd);
    }

    clock_time.PrintDuration();
    ProfilerStop();

    destroyGlobalEngine();
    return 0;
}