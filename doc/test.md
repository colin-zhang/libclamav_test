### 测试机器
```
Intel(R) Xeon(R) CPU E5-2650 v4 @ 2.20GHz
CentOS Linux release 7.4.1708 (Core)
```
###### libclamav
```
clamav version:devel-clamav-0.99-beta1-954-ged309f5be, virus num:6514207

```
[https://github.com/Cisco-Talos/clamav-devel](https://github.com/Cisco-Talos/clamav-devel)

###### libclamav wich libpcre

```
[root@localhost libclamav_test]# ./av_test
clamav version=devel-clamav-0.99-beta1-954-ged309f5
name=pre/sample/hdp_zhibo.apk
	size=2803639,ret=0 , stat=10 , type[CL_TYPE_BINARY_DATA], virname[Andr.Malware.Agent-5869333-0]
name=pre/sample/yumizhibo.apk
	size=10813396,ret=0 , stat=11 , type[CL_TYPE_BINARY_DATA], virname[Andr.Malware.Agent-5869333-0]
name=pre/sample/shoujigaoqingdianshi.apk
	size=9029088,ret=0 , stat=11 , type[CL_TYPE_GRAPHICS], virname[Andr.Malware.Agent-5869333-0]
[28s, 359ms, 725us, 193ns], [28359725.193us], [62392.426897M cycles]

```

###### libclamav wich libpcre2-8
```
[root@localhost libclamav_test]# ./av_test -f
clamav version=devel-clamav-0.99-beta1-954-ged309f5
name=pre/sample/hdp_zhibo.apk
	size=2803639,ret=0 , stat=10 , type[CL_TYPE_BINARY_DATA], virname[Andr.Malware.Agent-5869333-0]
name=pre/sample/yumizhibo.apk
	size=10813396,ret=0 , stat=11 , type[CL_TYPE_BINARY_DATA], virname[Andr.Malware.Agent-5869333-0]
name=pre/sample/shoujigaoqingdianshi.apk
	size=9029088,ret=0 , stat=11 , type[CL_TYPE_GRAPHICS], virname[Andr.Malware.Agent-5869333-0]
[27s, 300ms, 901us, 187ns], [27300901.187us], [60062.965403M cycles]
PROFILE: interrupts/evictions/bytes = 2729/1146/275136

```

