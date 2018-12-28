

// Util.cpp
// disable deprecation
#pragma warning(disable:4996)


#include "common.h"


	// to add an interval to a SYSTEMTIME structure
	//  this is the recommended method
	//  see "Programming Windows Applications" - Jeff Richter
	void
AddTimeSeconds(SYSTEMTIME & stTime , int Interval)
	{
	FILETIME ftTime;
	LARGE_INTEGER liTime;
	LARGE_INTEGER liMsInterval;

	// convert from SYSTEMTIME to FILETIME
	SystemTimeToFileTime(&stTime, &ftTime);

	// convert from FILETIME to large integer
	liTime.HighPart = ftTime.dwHighDateTime;
	liTime.LowPart  = ftTime.dwLowDateTime;

	// there are 10,000  100 nanosecond units  in one millisecond.
	//  we must use 64 bit multiplication because 5 minutes
	//  is equivalent to 30,000,000,000
	//  100 nanosecond units. and is too big
	//  for a 32 bit number to hold.
	liMsInterval.QuadPart = Int32x32To64(Interval, 1000 * 10000);

	// add the specified interval.
	liTime.QuadPart += liMsInterval.QuadPart;

	// convert from large integer to FILETIME
	ftTime.dwHighDateTime = liTime.HighPart;
	ftTime.dwLowDateTime  = liTime.LowPart;

	// convert from FILETIME to SYSTEMTIME
	FileTimeToSystemTime(&ftTime, &stTime);
	}


	char
DialogicStateName[272][30] =
	{
		{"ST_ONHK"},			//   0
		{"ST_IDLE"},			//   1
		{"ST_OFFHK"},			//   2
		{"ST_HELLO_MSG"},		//   3
		{"ST_GET_EXT"},			//   4
		{"ST_NO_LOG_MSG"},		//   5
		{"ST_PLAY_PROMPT"},		//   6
		{"ST_CB_MSG"},			//   7
		{"ST_CB_MSG2"},			//   8
		{"ST_CB_MSG3"},			//   9

		{"ST_IMPAIRMENT_MSG"},	//  10
		{"ST_GET_DIS"},			//  11
		{"ST_GOODBYE_MSG"},		//  12
		{"ST_GOODBYE3_MSG"},	//  13
		{"ST_ADD_PAGE"},		//  14
		{"ST_ANOTHER"},			//  15
		{"ST_ANOTHER_GET"},		//  16
		{"ST_DID_GET_EXT"},		//  17
		{"ST_DID_HKFLSH"},		//  18
		{"ST_EXCP_TIME"},		//  19

		{"ST_CB_VOICE"},		//  20
		{"ST_GET_VOICE"},		//  21
		{"ST_NO_VOICE"},		//  22
		{"ST_023"},				//  23
		{"ST_024"},				//  24
		{"ST_VP_PAGE"},			//  25
		{"ST_VP_IDLE"},			//  26
		{"ST_VOICE_DTMF"},		//  27
		{"ST_T1_PAUSE"},		//  28
		{"ST_029"},				//  29

		{"ST_LSTNG_MSG"},		//  30
		{"ST_LSTOK_MSG"},		//  31
		{"ST_LSTOK_VCE"},		//  32
		{"ST_LST_REC_OK"},		//  33
		{"ST_LST_RECORD"},		//  34
		{"ST_LST_GET_DIS"},		//  35
		{"ST_LST_SPK_MSG"},		//  36
		{"ST_037"},				//  37
		{"ST_038"},				//  38
		{"ST_039"},				//  39

		{"ST_GET_CVR_RES"},		//  40
		{"ST_CVR_RECUR"},		//  41
		{"ST_042"},				//  42
		{"ST_043"},				//  43
		{"ST_044"},				//  44
		{"ST_045"},				//  45
		{"ST_046"},				//  46
		{"ST_047"},				//  47
		{"ST_048"},				//  48
		{"ST_CHECK_IMPAIRED_VENDOR_ANN"},	//  49

		{"ST_ONHK_DELAY"},		//  50
		{"ST_CCPG_CALL"},		//  51
		{"ST_CCPG_ERR"},		//  52
		{"ST_053"},				//  53
		{"ST_054"},				//  54
		{"ST_055"},				//  55
		{"ST_056"},				//  56
		{"ST_057"},				//  57
		{"ST_058"},				//  58
		{"ST_059"},				//  59

		{"ST_PLAY_ID"},			//  60
		{"ST_GOODBYE_PROMPT"},	//  61
		{"ST_GOODBYE_ID"},		//  62
		{"ST_GOODBYE2_MSG"},	//  63
		{"ST_GREETING1_MSG"},	//  64
		{"ST_GREETING2_MSG"},	//  65
		{"ST_066"},				//  66
		{"ST_067"},				//  67
		{"ST_068"},				//  68
		{"ST_069"},				//  69

		{"ST_CATS_MSG"},		//  70
		{"ST_CATS_REC_ON"},		//  71
		{"ST_CATS_REC_OF"},		//  72
		{"ST_CATS_V_ON"},		//  73
		{"ST_CATS_PAGE"},		//  74
		{"ST_CATS_V_OFF"},		//  75
		{"ST_CATS_CALL"},		//  76
		{"ST_CATS_GET_DT"},		//  77
		{"ST_CATS_PLAY"},		//  78
		{"ST_CATS_ONHK"},		//  79

		{"ST_CATS_ABORT"},		//  80
		{"ST_CATS_REC_NG"},		//  81
		{"ST_CATS_REC_OK"},		//  82
		{"ST_083"},				//  83
		{"ST_084"},				//  84
		{"ST_085"},				//  85
		{"ST_086"},				//  86
		{"ST_087"},				//  87
		{"ST_088"},				//  88
		{"ST_MENU_STAT_2"},		//  89

		{"ST_MENU_PASS"},		//  90
		{"ST_MENU_PSCHK"},		//  91
		{"ST_MENU_PSERR"},		//  92
		{"ST_MENU_STAT"},		//  93
		{"ST_MENU_MSG"},		//  94
		{"ST_MENU_CHK"},		//  95
		{"ST_096"},				//  96
		{"ST_MENU_HELLO"},		//  97
		{"ST_MENU_V_PRMPT"},	//  98
		{"ST_MENU_I_PRMPT"},	//  99

		{"ST_OP_TRANSFER"},		//  100
		{"ST_OP_TRANSFER2"},	//  101
		{"ST_102"},				//  102
		{"ST_103"},				//  103
		{"ST_104"},				//  104
		{"ST_105"},				//  105
		{"ST_106"},				//  106
		{"ST_107"},				//  107
		{"ST_108"},				//  108
		{"ST_109"},				//  109

		{"ST_110"},				//  110
		{"ST_111"},				//  111
		{"ST_112"},				//  112
		{"ST_113"},				//  113
		{"ST_114"},				//  114
		{"ST_115"},				//  115
		{"ST_116"},				//  116
		{"ST_117"},				//  117
		{"ST_118"},				//  118
		{"ST_119"},				//  119

		{"ST_STAT_V_PRMPT"},	//  120
		{"ST_STAT_I_PRMPT"},	//  121
		{"ST_STAT_HELLO"},		//  122
		{"ST_STAT_CHANGE2"},	//  123
		{"ST_STAT_MSG6"},		//  124
		{"ST_STAT_MSG7"},		//  125
		{"ST_STAT_MSG4"},		//  126
		{"ST_STAT_MSG5"},		//  127
		{"ST_STAT_MSG"},		//  128
		{"ST_STAT_MSG2"},		//  129

		{"ST_STAT_MSG3"},		//  130
		{"ST_STAT_NEXT"},		//  131
		{"ST_STAT_PASS"},		//  132
		{"ST_STAT_PSCHK"},		//  133
		{"ST_STAT_PSERR"},		//  134
		{"ST_STAT_TYPES"},		//  135
		{"ST_STAT_CHANGE"},		//  136
		{"ST_STAT_WRITE"},		//  137
		{"ST_138"},				//  138
		{"ST_139"},				//  139

		{"ST_STAT_C_MSG"},		//  140
		{"ST_STAT_C_GET"},		//  141
		{"ST_STAT_C_NG1"},		//  142
		{"ST_STAT_C_NG2"},		//  143
		{"ST_STAT_C_NG3"},		//  144
		{"ST_STAT_C_NG4"},		//  145
		{"ST_STAT_WRITE2"},		//  146
		{"ST_STAT_C_NG5"},		//  147
		{"ST_148"},				//  148
		{"ST_149"},				//  149

		{"ST_STAT_F_MSG"},		//  150
		{"ST_STAT_F_GET"},		//  151
		{"ST_152"},				//  152
		{"ST_153"},				//  153
		{"ST_154"},				//  154
		{"ST_STAT_M_MSG"},		//  155
		{"ST_STAT_M_GET"},		//  156
		{"ST_STAT_M_MSG2"},		//  157
		{"ST_STAT_M_SRCH"},		//  158
		{"ST_STAT_M_NO"},		//  159

		{"ST_STAT_M_DATE"},		//  160
		{"ST_STAT_M_DATE2"},	//  161
		{"ST_STAT_M_DATE3"},	//  162
		{"ST_STAT_M_DATE4"},	//  163
		{"ST_STAT_M_TALK"},		//  164
		{"ST_STAT_M_TALK2"},	//  165
		{"ST_STAT_M_VOICE"},	//  166
		{"ST_STAT_M_TEXT"},		//  167
		{"ST_STAT_M_MSG3"},		//  168
		{"ST_STAT_M_GET3"},		//  169

		{"ST_STAT_M_NO2"},		//  170
		{"ST_STAT_M_PASS"},		//  171
		{"ST_STAT_M_PSCHK"},	//  172
		{"ST_STAT_M_PSERR"},	//  173
		{"ST_STAT_M_DATE5"},	//  174
		{"ST_STAT_M_STAT"},		//  175
		{"ST_STAT_M_DATE6"},	//  176
		{"ST_STAT_M_CVR"},		//  177
		{"ST_178"},				//  178
		{"ST_179"},				//  179

		{"ST_STAT_RFD_MSG"},	//  180
		{"ST_STAT_RFD_GET"},	//  181
		{"ST_STAT_RFD_NG"},		//  182
		{"ST_183"},				//  183
		{"ST_184"},				//  184
		{"ST_VCE_MSG"},			//  185
		{"ST_VCE_MSG2"},		//  186
		{"ST_VCE_MSG3"},		//  187
		{"ST_VCE_MSG4"},		//  188
		{"ST_VCE_NEXT"},		//  189

		{"ST_VCE_PASS"},		//  190
		{"ST_VCE_PSCHK"},		//  191
		{"ST_VCE_PSERR"},		//  192
		{"ST_VCE_CHG_MSG"},		//  193
		{"ST_VCE_CHANGE"},		//  194
		{"ST_195"},				//  195
		{"ST_VPAGE_TALK"},		//  196
		{"ST_VPAGE_DONE"},		//  197
		{"ST_198"},				//  198
		{"ST_199"},				//  199

		{"ST_PASS_MSG"},		//  200
		{"ST_PASS_NEXT"},		//  201
		{"ST_PASS_PASS"},		//  202
		{"ST_PASS_PSCHK"},		//  203
		{"ST_PASS_PSERR"},		//  204
		{"ST_PASS_MSG2"},		//  205
		{"ST_PASS_GET"},		//  206
		{"ST_PASS_WRITE"},		//  207
		{"ST_208"},				//  208
		{"ST_209"},				//  209

		{"ST_210"},				//  210
		{"ST_211"},				//  211
		{"ST_212"},				//  212
		{"ST_213"},				//  213
		{"ST_214"},				//  214
		{"ST_215"},				//  215
		{"ST_216"},				//  216
		{"ST_217"},				//  217
		{"ST_218"},				//  218
		{"ST_219"},				//  219

		{"ST_PARK_MSG"},		//  220
		{"ST_PARK_GET"},		//  221
		{"ST_PARK_TMSG"},		//  222
		{"ST_PARK_TRSFR"},		//  223
		{"ST_PARK_RECALL"},		//  224
		{"ST_PARK_NG_MSG"},		//  225
		{"ST_226"},				//  226
		{"ST_227"},				//  227
		{"ST_228"},				//  228
		{"ST_229"},				//  229

		{"ST_230"},				//  230
		{"ST_231"},				//  231
		{"ST_232"},				//  232
		{"ST_233"},				//  233
		{"ST_234"},				//  234
		{"ST_235"},				//  235
		{"ST_236"},				//  236
		{"ST_237"},				//  237
		{"ST_238"},				//  238
		{"ST_239"},				//  239

		{"ST_NAME_MSG"},		//  240
		{"ST_NAME_GET"},		//  241
		{"ST_NAME_SELECT"},		//  242
		{"ST_NAME_SELECT2"},	//  243
		{"ST_NAME_CHOOSE"},		//  244
		{"ST_NAME_MSG2"},		//  245
		{"ST_NAME_MSG3"},		//  246
		{"ST_247"},				//  247
		{"ST_248"},				//  248
		{"ST_249"},				//  249

		{"ST_STAT_EX_MSG1"},	//  250
		{"ST_STAT_EX_GET1"},	//  251
		{"ST_STAT_EX_MSG2"},	//  252
		{"ST_STAT_EX_GET2"},	//  253
		{"ST_STAT_EX_AMPM"},	//  254
		{"ST_STAT_EX_GET3"},	//  255
		{"ST_STAT_EX_ERR"},		//  256
		{"ST_257"},				//  257
		{"ST_258"},				//  258
		{"ST_259"},				//  259

		{"ST_STAT_C_NG6"},		//  260
		{"ST_261"},				//  261
		{"ST_262"},				//  262
		{"ST_263"},				//  263
		{"ST_264"},				//  264
		{"ST_265"},				//  265
		{"ST_266"},				//  266
		{"ST_267"},				//  267
		{"ST_268"},				//  268
		{"ST_269"},				//  269

		{"ST_VM_TR_MSG"},		//  270
		{"ST_VM_TRANSFER"},		//  271
	};

