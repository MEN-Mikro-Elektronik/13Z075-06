<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
  </compound>
  <compound kind="file">
    <name>z75_doc.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z075_SPEED/DRIVER/COM/</path>
    <filename>z75__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>z75_drv.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z075_SPEED/DRIVER/COM/</path>
    <filename>z75__drv_8c</filename>
    <member kind="function">
      <type>void</type>
      <name>Z75_GetEntry</name>
      <anchor>a1</anchor>
      <arglist>(LL_ENTRY *drvP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Init</name>
      <anchor>a2</anchor>
      <arglist>(DESC_SPEC *descP, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Exit</name>
      <anchor>a3</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Read</name>
      <anchor>a4</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *valueP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Write</name>
      <anchor>a5</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_SetStat</name>
      <anchor>a6</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 code, int32 ch, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_GetStat</name>
      <anchor>a7</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 code, int32 ch, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_BlockRead</name>
      <anchor>a8</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_BlockWrite</name>
      <anchor>a9</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Irq</name>
      <anchor>a10</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Info</name>
      <anchor>a11</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a12</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a13</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>GetFreq</name>
      <anchor>a14</anchor>
      <arglist>(LL_HANDLE *llHdl, u_int32 *freqP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>automatedTsk</name>
      <anchor>a15</anchor>
      <arglist>(void *hdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>checkRange</name>
      <anchor>a16</anchor>
      <arglist>(u_int32 clk, u_int32 per)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>const char</type>
      <name>IdentString</name>
      <anchor>a0</anchor>
      <arglist>[]</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>z75_int.h</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z075_SPEED/DRIVER/COM/</path>
    <filename>z75__int_8h</filename>
    <class kind="struct">LL_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>Z075_RESULT_ADDR_OFFSET</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_CTRL_STS_ADDR_OFFSET</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_RES_MEAS_TIME_MSK</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_RES_PERIOD_MSK</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_RES_NEW_VALUE_MSK</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_STS_LOW_MSK</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_STS_HIGH_MSK</name>
      <anchor>a13</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_STS_EN_MSK</name>
      <anchor>a14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_STS_RES_MSK</name>
      <anchor>a15</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_DESC_DEF_SYSCLK</name>
      <anchor>a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_DESC_DEF_RESOLUTION</name>
      <anchor>a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_DESC_DEF_UPLTD</name>
      <anchor>a18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z075_DESC_DEV_LOWLTD</name>
      <anchor>a19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_NO_LL_HANDLE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_NUMBER</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_IRQ</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_COUNT</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_SIZE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBG_MYLEVEL</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBH</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Init</name>
      <anchor>a20</anchor>
      <arglist>(DESC_SPEC *descSpec, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Exit</name>
      <anchor>a21</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Read</name>
      <anchor>a22</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Write</name>
      <anchor>a23</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_SetStat</name>
      <anchor>a24</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_GetStat</name>
      <anchor>a25</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_BlockRead</name>
      <anchor>a26</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_BlockWrite</name>
      <anchor>a27</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Irq</name>
      <anchor>a28</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z75_Info</name>
      <anchor>a29</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a30</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a31</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>GetFreq</name>
      <anchor>a32</anchor>
      <arglist>(LL_HANDLE *llHdl, u_int32 *freqP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>automatedTsk</name>
      <anchor>a33</anchor>
      <arglist>(void *hdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int</type>
      <name>checkRange</name>
      <anchor>a34</anchor>
      <arglist>(u_int32 clk, u_int32 per)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LL_HANDLE</name>
    <filename>structLL__HANDLE.html</filename>
    <member kind="variable">
      <type>int32</type>
      <name>memAlloc</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_HANDLE *</type>
      <name>osHdl</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_IRQ_HANDLE *</type>
      <name>irqHdl</name>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DESC_HANDLE *</type>
      <name>descHdl</name>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MACCESS</type>
      <name>ma</name>
      <anchor>o4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MDIS_IDENT_FUNCT_TBL</type>
      <name>idFuncTbl</name>
      <anchor>o5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_ALARM_HANDLE *</type>
      <name>alarmHdl</name>
      <anchor>o6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>dbgLevel</name>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBG_HANDLE *</type>
      <name>dbgHdl</name>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>irqCount</name>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>fpgaSysClk</name>
      <anchor>o10</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>upLimit</name>
      <anchor>o11</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>lowLimit</name>
      <anchor>o12</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int8</type>
      <name>autoMode</name>
      <anchor>o13</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int16</type>
      <name>res</name>
      <anchor>o14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int16</type>
      <name>startFlag</name>
      <anchor>o15</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_SIG_HANDLE *</type>
      <name>lowSigIdP</name>
      <anchor>o16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_SIG_HANDLE *</type>
      <name>upSigIdP</name>
      <anchor>o17</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>realMsec</name>
      <anchor>o18</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>dummy</name>
    <title></title>
    <filename>dummy</filename>
  </compound>
</tagfile>
