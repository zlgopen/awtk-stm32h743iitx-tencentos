# AWTK 针对 STM32H743 的移植。

* [AWTK](https://github.com/zlgopen/awtk) 全称 Toolkit AnyWhere，是 [ZLG](http://www.zlg.cn/) 开发的开源 GUI 引擎，旨在为嵌入式系统、WEB、各种小程序、手机和 PC 打造的通用 GUI 引擎，为用户提供一个功能强大、高效可靠、简单易用、可轻松做出炫酷效果的 GUI 引擎。

[awtk-stm32h743iitx-tencentos](https://github.com/zlgopen/awtk-stm32h743iitx-tencentos) 是 AWTK 在 TM32H743 上的移植。

本项目以 [正点原子阿波罗STM32H743开发板STM32H7](https://item.taobao.com/item.htm?id=571115692251&ali_refid=a3_430582_1006:1103191143:N:HIGgPtZ%2BAhQpeEvXe%2FK1RspPjJRVCvOC:5b41a06d9f58d787690cd30a8865e887&ali_trackid=1_5b41a06d9f58d787690cd30a8865e887&spm=a230r.1.14.8#detail) 为载体移植，其它开发板可能要做些修改，有问题请请创建 issue。

## 编译

1. 获取源码

```
git clone https://github.com/zlgopen/awtk-stm32h743iitx-tencentos.git
cd awtk-stm32h743iitx-tencentos
git clone https://github.com/zlgopen/awtk.git
git clone https://github.com/zlgopen/awtk-fs-adapter.git
```

2. 用 keil 打开 USER/awtk.uvprojx

## 文档

* [AWTK 在 NSTM32H743 移植笔记](https://github.com/zlgopen/awtk-stm32h743iitx-tencentos/blob/master/docs/stm32h743iitx_port.md)