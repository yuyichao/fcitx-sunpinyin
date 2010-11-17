#include <stdlib.h>
#include <fcitx/im.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcitx-config/hotkey.h>
#include <ime-core/imi_view.h>
#include <ime-core/imi_options.h>
#include <ime-core/utils.h>
#include <fcitx-config/configfile.h>
#include <fcitx-config/profile.h>
#include <fcitx-config/xdg.h>
#include <string>

#include "handler.h"
#include "eim.h"

#define _(x) (x)

#ifdef __cplusplus
extern "C" {
#endif
    EXTRA_IM EIM = {
        _("Sunpinyin"), /* Name */
        "fcitx-sunpinyin", /* IconName */
        Reset, /* Reset */
        DoInput, /* DoInput */
        GetCandWords, /* GetCandWords */
        GetCandWord,
        Init,
        Destroy,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        NULL,
        NULL
    };
#ifdef __cplusplus
}
#endif

static ConfigFileDesc* GetSunpinyinConfigDesc();
static void LoadConfig(Bool reload = False);
static void SaveConfig();

static FcitxWindowHandler* instance = NULL;
static CIMIView* view = NULL;
static ConfigFileDesc* sunpinyinConfigDesc;
static FcitxSunpinyinConfig fs;

static const char* fuzzyPairs[][2] = {
    {"sh", "s"},
    {"zh", "z"},
    {"ch", "c"},
    {"an", "ang"},
    {"on", "ong"},
    {"en", "eng"},
    {"in", "ing"},
    {"eng", "ong"},
    {"ian", "iang"},
    {"uan", "uang"},
    {"n", "l"},
    {"f", "h"},
    {"l", "r"},
    {"k", "g"}
};

static const char *correctionPairs[][2] = {
    {"ign", "ing"},
    {"ogn", "ong"},
    {"uen", "un"},
    {"img", "ing"},
    {"iou", "iu"},
    {"uei", "ui"}
};

__EXPORT_API
void Reset (void)
{
    FcitxProfile *profile = (FcitxProfile*) EIM.profile;
    view->setStatusAttrValue(CIMIWinHandler::STATUS_ID_FULLSYMBOL, profile->bCorner);
    view->setStatusAttrValue(CIMIWinHandler::STATUS_ID_FULLPUNC, profile->bChnPunc);
    view->clearIC();
}

__EXPORT_API
INPUT_RETURN_VALUE DoInput (unsigned int keycode, unsigned int state, int count)
{
    if ((keycode <= 0x20 || keycode > 0x7E || keycode == 0x3b ) && view->getIC()->isEmpty())
        return IRV_TO_PROCESS;

    if (keycode == 0xFF8D)
        keycode -= 0x80;

    instance->commit_flag = false;
    instance->candidate_flag = false;
    unsigned int changeMasks = view->onKeyEvent(CKeyEvent(keycode, keycode, state));

    if (instance->commit_flag)
        return IRV_GET_CANDWORDS;
    if (!(changeMasks & CIMIView::KEYEVENT_USED))
        return IRV_TO_PROCESS;

    if (instance->candidate_flag)
    {
        return IRV_DISPLAY_CANDWORDS;
    }

    return IRV_TO_PROCESS;
}

__EXPORT_API
INPUT_RETURN_VALUE GetCandWords(SEARCH_MODE searchMode)
{
    return IRV_DO_NOTHING;
}

__EXPORT_API
char *GetCandWord (int iIndex)
{
    EIM.CandWordCount = 0;
    instance->commit_flag = false;
    instance->candidate_flag = false;
    if (iIndex <= 8)
    {
        unsigned int keycode = '1' + iIndex;
        unsigned int state = 0;
        unsigned int changeMasks = view->onKeyEvent(CKeyEvent(keycode, keycode, state));

        if (instance->commit_flag)
            return EIM.StringGet;
    }
    
    return NULL;
}

__EXPORT_API
int Init (char *arg)
{
    FcitxConfig *fc = (FcitxConfig*)EIM.fc;

    LoadConfig();

    CSunpinyinSessionFactory& fac = CSunpinyinSessionFactory::getFactory();

    if (fs.bUseShuangpin)
        fac.setPinyinScheme(CSunpinyinSessionFactory::SHUANGPIN);
    else
        fac.setPinyinScheme(CSunpinyinSessionFactory::QUANPIN);

    AShuangpinSchemePolicy::instance().setShuangpinType(fs.SPScheme);
    AQuanpinSchemePolicy::instance().setFuzzySegmentation(fs.bFuzzySegmentation);
    AQuanpinSchemePolicy::instance().setInnerFuzzySegmentation(fs.bFuzzyInnerSegmentation);
    view = fac.createSession();

    instance = new FcitxWindowHandler();
    view->getIC()->setCharsetLevel(1);// GBK

    view->setCandiWindowSize(fc->iMaxCandWord);
    view->attachWinHandler(instance);
    // page up/down key
    CHotkeyProfile* prof = view->getHotkeyProfile();
    prof->clear();

    int i = 0;
    for (i = 0 ; i < 2; i++)
    {
        if (fc->hkPrevPage[i].iKeyCode)
            prof->addPageUpKey(CKeyEvent(fc->hkPrevPage[i].iKeyCode));
        if (fc->hkNextPage[i].iKeyCode)
            prof->addPageDownKey(CKeyEvent(fc->hkNextPage[i].iKeyCode));
    }
    
    string_pairs fuzzy, correction;
    for (i = 0; i < FUZZY_SIZE; i++)
        if (fs.bFuzzy[i])
            fuzzy.push_back(std::make_pair<std::string, std::string>(fuzzyPairs[i][0], fuzzyPairs[i][1]));
    
    for (i = 0; i < CORRECT_SIZE; i++)
        if (fs.bAutoCorrecting[i])
            correction.push_back(std::make_pair<std::string, std::string>(correctionPairs[i][0], correctionPairs[i][1]));
    
    if (fuzzy.size() != 0)
    {
        AQuanpinSchemePolicy::instance().setFuzzyForwarding(true);
        AQuanpinSchemePolicy::instance().setFuzzyPinyinPairs(fuzzy);
    }
    else
    {
        AQuanpinSchemePolicy::instance().setFuzzyForwarding(false);
        AQuanpinSchemePolicy::instance().clearFuzzyPinyinPairs();
    }
    
    if (correction.size() != 0)
    {
        AQuanpinSchemePolicy::instance().setAutoCorrecting(true);
        AQuanpinSchemePolicy::instance().setAutoCorrectionPairs(correction);
    }
    else
        AQuanpinSchemePolicy::instance().setAutoCorrecting(false);
        
    view->setCancelOnBackspace(1);
    instance->set_eim(&EIM);

    return 0;
}

__EXPORT_API
int Destroy (void)
{
    CSunpinyinSessionFactory& fac = CSunpinyinSessionFactory::getFactory();
    fac.destroySession(view);

    if (instance)
        delete instance;

    return 0;
}

ConfigFileDesc* GetSunpinyinConfigDesc()
{
    if (!sunpinyinConfigDesc)
    {
        FILE *tmpfp;
        tmpfp = GetXDGFileData("addon/fcitx-sunpinyin.desc", "r", NULL);
        sunpinyinConfigDesc = ParseConfigFileDescFp(tmpfp);
        fclose(tmpfp);
    }

    return sunpinyinConfigDesc;
}

void LoadConfig(Bool reload)
{
    ConfigFileDesc *configDesc = GetSunpinyinConfigDesc();

    FILE *fp = GetXDGFileUser( "addon/fcitx-sunpinyin.config", "rt", NULL);

    if (!fp)
    {
        if (!reload && errno == ENOENT)
        {
            SaveConfig();
            LoadConfig(True);
        }
        return;
    }
    ConfigFile *cfile = ParseConfigFileFp(fp, configDesc);

    if (cfile)
    {
        FcitxSunpinyinConfigConfigBind(&fs, cfile, configDesc);
        ConfigBindSync((GenericConfig*)&fs);
    }
    else
    {
        fs.bUseShuangpin = False;
        fs.SPScheme = MS2003;
        fs.bFuzzySegmentation = False;
        fs.bFuzzyInnerSegmentation = False;
        int i = 0;
        for (i = 0; i < FUZZY_SIZE; i ++)
            fs.bFuzzy[i] = False;
        
        for (i = 0; i < CORRECT_SIZE; i ++)
            fs.bAutoCorrecting[i] = False;
    }

}

void SaveConfig()
{
    ConfigFileDesc *configDesc = GetSunpinyinConfigDesc();
    FILE *fp = GetXDGFileUser( "addon/fcitx-sunpinyin.config", "wt", NULL);
    SaveConfigFileFp(fp, fs.gconfig.configFile, configDesc);
    fclose(fp);
}
