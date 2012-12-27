/*
 * Install script (NSIS) for Flash Card Screensaver
 */
!define PRODUCT     "Flash Card Screensaver"
!define PUBLISHER   "ALKOSoft"
!define BLD_MAIN    "..\..\build\release-win32"
!define BLD_STUB    "..\..\build\stub-win32"
!define BLD_LIBS    "..\..\build\libs-win32"
!define BLD_DICT    "..\..\build\dict"
!define REG_CONFIG  "Software\ALKOSoft\FCardSaver"
!define REG_UNINSTALL \
    "Software\Microsoft\Windows\CurrentVersion\Uninstall\FCardScreensaver"
!define PROD_ICON   "..\..\src\platform\fcardsaver.ico"

!include LogicLib.nsh
!include nsArray.nsh
!include nsDialogs.nsh

OutFile         "..\..\build\fcardss-install.exe"
SetCompressor   lzma
Icon            "${PROD_ICON}"
UninstallIcon   "${PROD_ICON}"
LicenseData     "gpl-3.0.txt"
InstallDir      "$PROGRAMFILES\${PRODUCT}"
Name            "${PRODUCT}"

ShowInstDetails show
ShowUnInstDetails show
RequestExecutionLevel admin

InstType "Full"
InstType "Local only"
InstType "Minimal"

# Installation pages
Page license
Page components "" "" "warnExternal"
Page directory
Page custom "extCompCreate" "extCompLeave"
Page instfiles

# Uninstallation pages
UninstPage uninstConfirm
UninstPage instfiles

Var DICT_LAST
Var DICT_PATH

Var EXT_CONFFILE
Var EXT_NAME
Var EXT_ID
Var EXT_TITLE
Var EXT_URL
Var EXT_FILE
Var EXT_LICENSE
Var EXT_DIR
Var EXT_LICLABEL
Var EXT_LICTEXT
Var EXT_LICCTRL

Function .onInit
    InitPluginsDir
    StrCpy $EXT_CONFFILE "$PLUGINSDIR\external.conf"
    File "/oname=$EXT_CONFFILE" "external.conf"
    Call initExtCompList
FunctionEnd

Section "${PRODUCT}"
    SectionIn RO
    CreateDirectory $INSTDIR

    WriteUninstaller "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "${REG_UNINSTALL}" "DisplayName" "${PRODUCT}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "Publisher" "${PUBLISHER}"
    WriteRegStr HKLM "${REG_UNINSTALL}" "UninstallString" \
        '"$INSTDIR\Uninstall.exe"'
    WriteRegStr HKLM "${REG_UNINSTALL}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "${REG_UNINSTALL}" "RunExecutable" "$INSTDIR\fcardsaver.exe"

    SetOutPath $INSTDIR
    File "${BLD_MAIN}\fcardsaver.exe"
    File "${BLD_LIBS}\libgcc_s_dw2-1.dll"
    File "${BLD_LIBS}\mingwm10.dll"
    File "${BLD_LIBS}\QtCore4.dll"
    File "${BLD_LIBS}\QtGui4.dll"

    SetOutPath $SYSDIR
    File "${BLD_STUB}\fcardss.scr"

    ReadRegDWORD $DICT_LAST HKCU "${REG_CONFIG}\cardsets" "size"
    ReadRegStr $DICT_PATH HKCU "${REG_CONFIG}\general" "data-root"
    ${If} $DICT_PATH == ""
        SetShellVarContext all
        StrCpy $DICT_PATH "$APPDATA\ALKOSoft\FCardSaver"
    ${EndIf}
    CreateDirectory "$DICT_PATH"
SectionEnd


!macro InstallDict dictfile separator
!if "${separator}" != ""
    StrCpy $2 "${separator}"
!endif
    StrCpy $1 "${dictfile}"
    Call InstallDict
!macroend
!define InstallDict "!insertmacro InstallDict"

SectionGroup /e "Flash card sets"
    Section "Japanese language card sets"
        SectionIn 1 2

        SetOutPath "$DICT_PATH"

        File "${BLD_DICT}\hiragana.tsv"
        File "${BLD_DICT}\hiragana-ext.tsv"
        File "${BLD_DICT}\hiragana-alien.tsv"
        File "${BLD_DICT}\katakana.tsv"
        File "${BLD_DICT}\katakana-ext.tsv"
        File "${BLD_DICT}\katakana-alien.tsv"

        File "${BLD_DICT}\kanjidic2.tsv"
        File "${BLD_DICT}\kanjidic2-G1.tsv"
        File "${BLD_DICT}\kanjidic2-G2.tsv"
        File "${BLD_DICT}\kanjidic2-G3.tsv"
        File "${BLD_DICT}\kanjidic2-G4.tsv"
        File "${BLD_DICT}\kanjidic2-G5.tsv"
        File "${BLD_DICT}\kanjidic2-G6.tsv"
        File "${BLD_DICT}\kanjidic2-jouyou.tsv"
        File "${BLD_DICT}\kanjidic2-jinmei.tsv"
        File "${BLD_DICT}\kanjidic2-itai.tsv"
        File "${BLD_DICT}\kanjidic2-hyougai.tsv"

        Push $1
        Push $2
        ${InstallDict} "hiragana.tsv"     "#0009"
        ${InstallDict} "katakana.tsv"     ""
        ${InstallDict} "kanjidic2-G1.tsv" ""
    SectionEnd
SectionGroupEnd

SectionGroup /e "Additional components (external downloads)" external_comp_id
    Section "KanjiStrokeOrders font" kanjistroke_id
        SectionIn 1

        nsArray::Get EXT_COMPONENTS ${kanjistroke_id}
        Pop $EXT_NAME
        StrCpy $1 "$PLUGINSDIR\$EXT_NAME"
        StrCpy $2 "*.ttf"
        Call InstallAllFonts
    SectionEnd

    Section "Hanazono font (A and B)" hanazono_id
        SectionIn 1

        nsArray::Get EXT_COMPONENTS ${hanazono_id}
        Pop $EXT_NAME
        StrCpy $1 "$PLUGINSDIR\$EXT_NAME"
        StrCpy $2 "*.ttf"
        Call InstallAllFonts
    SectionEnd
SectionGroupEnd

Function initExtCompList
    nsArray::Set EXT_COMPONENTS /key=${kanjistroke_id} "KanjiStrokeOrders"
    nsArray::Set EXT_COMPONENTS /key=${hanazono_id} "Hanazono"
FunctionEnd

Function extCompCreate
    Push $0

    nsDialogs::Create 1018
    Pop $0
    ${IfThen} $0 == error Abort
    ${NSD_CreateLabel} 0 0 100% 12u "License for..."
    Pop $EXT_LICLABEL
    nsDialogs::CreateControl RichEdit20A \
        ${WS_VISIBLE}|${WS_CHILD}|${WS_TABSTOP}|${WS_VSCROLL}|${ES_MULTILINE}|${ES_READONLY} \
        ${__NSD_Text_EXSTYLE} 0 16u 100% 100u ""
    Pop $EXT_LICTEXT
    ${NSD_CreateCheckBox} 0 120u 160u 12u "I accept"
    Pop $EXT_LICCTRL

    nsArray::Get EXT_COMPONENTS /reset
    Call extCompNext
    ${IfThen} ${Errors} Abort
    nsDialogs::Show

    Pop $0
FunctionEnd

Function extCompNext
    ${Do}
        nsArray::Get EXT_COMPONENTS /next
        ${If} ${Errors}
            SetErrors
            Return
        ${EndIf}
        Pop $EXT_ID
        Pop $EXT_NAME
        ${If} ${SectionIsSelected} $EXT_ID
            ClearErrors
            Call extCompLoad
            ${IfNotThen} ${Errors} Return
        ${EndIf}
    ${Loop}
FunctionEnd

Function extCompLoad
    SectionGetText $EXT_ID $EXT_TITLE
    ReadINIStr $EXT_URL "$EXT_CONFFILE" "$EXT_NAME" "url"
    ReadINIStr $EXT_FILE "$EXT_CONFFILE" "$EXT_NAME" "file"
    ReadINIStr $EXT_LICENSE "$EXT_CONFFILE" "$EXT_NAME" "license"
    StrCpy $EXT_DIR "$PLUGINSDIR\$EXT_NAME"
    CreateDirectory "$EXT_DIR"

    ClearErrors
    Call DownloadExtComponent
    ${If} ${Errors}
        SetErrors
        Return
    ${EndIf}
    ${Unless} ${FileExists} "$EXT_DIR\$EXT_LICENSE"
        SetErrors
        Return
    ${EndIf}
    ${NSD_SetText} $EXT_LICLABEL "License for $EXT_TITLE"
    nsRichEdit::Load $EXT_LICTEXT "$EXT_DIR\$EXT_LICENSE"
    ${NSD_SetState} $EXT_LICCTRL ${BST_UNCHECKED}
    ClearErrors
FunctionEnd

Function extCompLeave
    Push $0
    ${NSD_GetState} $EXT_LICCTRL $0
    ${Unless} $0 = ${BST_CHECKED}
        Pop $0
        MessageBox MB_ICONSTOP|MB_OK "You must accept this license to continue."
        Abort
    ${EndUnless}
    Pop $0
    ClearErrors
    Call extCompNext
    ${Unless} ${Errors}
        Abort
    ${EndUnless}
FunctionEnd

Section "Uninstall"
    Delete "$SYSDIR\fcardss.scr"

    Delete "$INSTDIR\fcardsaver.exe"
    Delete "$INSTDIR\libgcc_s_dw2-1.dll"
    Delete "$INSTDIR\mingwm10.dll"
    Delete "$INSTDIR\QtCore4.dll"
    Delete "$INSTDIR\QtGui4.dll"

    Delete "$INSTDIR\Uninstall.exe"
    RMDir $INSTDIR

    DeleteRegKey HKLM "${REG_UNINSTALL}"
SectionEnd

#################
### FUNCTIONS ###
#################

/*
 * Warn about external components, if any selected
 */
Function warnExternal
    ${If} ${SectionIsSelected} ${external_comp_id}
    ${OrIf} ${SectionIsPartiallySelected} ${external_comp_id}
        MessageBox MB_OKCANCEL|MB_ICONQUESTION \
            "You have selected to install some external components.$\nThese components will be downloaded from Internet,$\nand some or all of them may be under a different license." \
            /SD IDOK IDOK +2
            Abort
    ${EndIf}
FunctionEnd

/*
 * Download and uncompress external component.
 */
Function DownloadExtComponent
    Push $0

    Inetc::get /POPUP "" /CAPTION "$EXT_TITLE" "$EXT_URL" "$EXT_DIR\$EXT_FILE" /END
    Pop $0
    ${If} $0 S!= "OK"
        Abort "Download failed: $0"
    ${EndIf}
    nsUnzip::Extract "/d=$EXT_DIR" "$EXT_DIR\$EXT_FILE" /END
    Pop $0
    ${If} $0 <> 0
        Abort "Extraction failed: $0"
    ${EndIf}

    Pop $0
FunctionEnd

/*
 * Install all fonts from directory.
 *
 *      $1:     Directory to look in.
 *      $2:     Font file name pattern.
 */
Function InstallAllFonts
    Push $0
    Push $R0
    Push $R1
    Push $R2

    FindFirst $0 $R1 "$1\$2"
    ${While} $R1 S!= ""
        CopyFiles /FILESONLY "$1\$R1" "$FONTS"
        System::Call 'gdi32::AddFontResource(t)i("$FONTS\$R1").R0?e'
        Pop $R2
        ${If} $R0 = 0
            Abort "Failed to register font '$R1': $R2"
        ${EndIf}
        FindNext $0 $R1
    ${EndWhile}
    FindClose $R1

    Pop $R2
    Pop $R1
    Pop $R0
    Pop $0
FunctionEnd

/*
 * Check whether flash card file is installed for display.
 * Sets errors flag if flash cards file is not installed.
 * Returns index in $0.
 *
 *      $1:     File name (with or without path).
 */
Function CheckDict
    Push $R0
    ${For} $0 1 $DICT_LAST
        ReadRegStr $R0 HKCU "${REG_CONFIG}\cardsets\$0" "path"
        ${If} $R0 == $1
            ${ExitFor}
        ${EndIf}
    ${Next}
    Pop $R0
    ${IfThen} $0 > $DICT_LAST SetErrors
FunctionEnd

/*
 * Install flash card file for display.
 *
 *      $1:     File name (with or without path).
 *      $2:     Separator.
 */
Function InstallDict
    Push $0
    Call CheckDict
    ${If} ${Errors}
        IntOp $DICT_LAST $DICT_LAST + 1
        WriteRegDWORD HKCU "${REG_CONFIG}\cardsets" "size" $DICT_LAST
        StrCpy $0 $DICT_LAST
    ${EndIf}
    WriteRegStr HKCU "${REG_CONFIG}\cardsets\$0" "path" "$1"
    WriteRegStr HKCU "${REG_CONFIG}\cardsets\$0" "separator" "$2"
    Pop $0
FunctionEnd
