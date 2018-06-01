#ifndef CLAMAV_ENGIN_H_
#define CLAMAV_ENGIN_H_

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <string>
#include <map>
#include <vector>
#include "clamav.h"

enum ClamavScanStat
{
    kScanStatIsVIRUS = 10,
    kScanStatNotVIRUS,
    kScanStatUnknow,
};

struct ClamavScanResult
{
    const char *virname;
    unsigned long int size;
    uint8_t stat;
    char type[40];
    ClamavScanResult()
    : virname("")
    , size(0)
    , stat(0)
    {
        //type[0] = 0;
        memset(type, 0, sizeof(type));
    }
};

//TODO, enable more settings
struct ClamavSettings
{
    long long max_file_size;
    long long max_scan_size;

    int ReMatchAlgorithm;

    std::string tmp_file_dir;

    ClamavSettings()
    : max_file_size(0)
    , max_scan_size(0)
    , ReMatchAlgorithm(kReMatchAlgorithm_None)\
    , tmp_file_dir("")
    {

    }
};

// ClamAV Virus Database : CVD
struct ClamavVDInfo
{
    std::string name;
    struct cl_cvd* info;
};

static inline std::string CVDInfoList2String(std::vector<ClamavVDInfo>& v)
{

    std::vector<ClamavVDInfo>::iterator it;
    std::string res_str;
    char buf[1024];
    for (it = v.begin(); it != v.end(); it++) 
    {
        snprintf(buf, sizeof buf,
                         "vd name :%s\n"
                         "time    :%s\n"
                         "version :%u\n"
                         "sigs    :%u\n"
                         "fl      :%u\n"
                         "md5     :%s\n"
                         "builder :%s\n"
                         "stime   :%u\n\n"
                         , it->name.c_str()
                         , it->info->time
                         , it->info->version
                         , it->info->sigs
                         , it->info->fl
                         , it->info->md5
                         , it->info->builder
                         , it->info->stime
            );
        res_str += std::string(buf);
    } 
    return res_str;
}

//typedef off_t (*clcb_pread)(void* handle, void *buf, size_t count, off_t offset)

class ClamavEngine
{
public:
    explicit ClamavEngine(const char* path);
    ~ClamavEngine();

    static std::string getClamavVersion();
    static void enableDebug();

    
    int buildEngine(ClamavSettings* av_settings = NULL);

    int reBuildEngine(ClamavSettings* av_settings = NULL);

    int ScanFmap(void* ptr, size_t len, ClamavScanResult* result, uint32_t scan_opt);

    int scanFileFd(int fd, ClamavScanResult* result, uint32_t scan_opt);

    int scanFileFdRaw(int fd, ClamavScanResult* result);

    int scanFileFdStd(int fd, ClamavScanResult* result);

    int getSettings();
    int setSettings(ClamavSettings* settings);

    int checkDatebaseChanged();

    uint32_t scanSettingMask(const char* setting_section);
    
    const std::string&   getErrInfo() 
    {
        return err_info_;
    }

    uint64_t getSigNum() 
    {
        return sig_num_;
    }

    std::vector<ClamavVDInfo>& getCVDInfoList()
    {
        return cvd_info_list_;
    }

private:
    struct cmp_str 
    {  
        bool operator()(const char* a, const char* b)   const
        {  
            return strcmp(a, b) < 0;  
        }  
    }; 

    void settingMaskInit();
    std::map<const char*, uint32_t, cmp_str> setting_mask_map_; 
private:
    std::string err_info_;
    uint64_t sig_num_;
private:
    std::string db_path_;
    std::vector<ClamavVDInfo> cvd_info_list_;
    struct cl_engine* engine_;
    struct cl_stat* dbstat_;
};

#endif
