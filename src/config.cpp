#include "eim.h"

CONFIG_BINDING_BEGIN(FcitxSunpinyinConfig);
CONFIG_BINDING_REGISTER("Sunpinyin", "UseShuangpin", bUseShuangpin);
CONFIG_BINDING_REGISTER("Sunpinyin", "ShuangpinScheme", SPScheme);
CONFIG_BINDING_REGISTER("Sunpinyin", "FuzzySegmentation", bFuzzySegmentation);
CONFIG_BINDING_REGISTER("Sunpinyin", "FuzzyInnerSegmentation", bFuzzyInnerSegmentation);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyZhiZi", bFuzzy[FUZZY_INDEX_ZhiZi]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyChiCi", bFuzzy[FUZZY_INDEX_ChiCi]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyShiSi", bFuzzy[FUZZY_INDEX_ShiSi]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyAnAng", bFuzzy[FUZZY_INDEX_AnAng]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyOnOng", bFuzzy[FUZZY_INDEX_OnOng]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyEnEng", bFuzzy[FUZZY_INDEX_EnEng]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyInIng", bFuzzy[FUZZY_INDEX_InIng]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyEngOng", bFuzzy[FUZZY_INDEX_EngOng]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyIanIang", bFuzzy[FUZZY_INDEX_IanIang]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyUanUang", bFuzzy[FUZZY_INDEX_UanUang]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyNeLe", bFuzzy[FUZZY_INDEX_NeLe]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyFoHe", bFuzzy[FUZZY_INDEX_FoHe]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyLeRi", bFuzzy[FUZZY_INDEX_LeRi]);
CONFIG_BINDING_REGISTER("QuanPin", "FuzzyKeGe", bFuzzy[FUZZY_INDEX_KeGe]);
CONFIG_BINDING_REGISTER("QuanPin", "AutoCorrectingIgnIng", bAutoCorrecting[CORRECT_INDEX_IgnIng]);
CONFIG_BINDING_REGISTER("QuanPin", "AutoCorrectingOgnOng", bAutoCorrecting[CORRECT_INDEX_OgnOng]);
CONFIG_BINDING_REGISTER("QuanPin", "AutoCorrectingUenUn", bAutoCorrecting[CORRECT_INDEX_UenUn]);
CONFIG_BINDING_REGISTER("QuanPin", "AutoCorrectingImgIng", bAutoCorrecting[CORRECT_INDEX_ImgIng]);
CONFIG_BINDING_REGISTER("QuanPin", "AutoCorrectingIouIu", bAutoCorrecting[CORRECT_INDEX_IouIu]);
CONFIG_BINDING_REGISTER("QuanPin", "AutoCorrectingUeiUi", bAutoCorrecting[CORRECT_INDEX_UeiUi]);
CONFIG_BINDING_END();