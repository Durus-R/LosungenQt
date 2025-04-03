Outfile "LosungenQtInstaller.exe"
InstallDir "$PROGRAMFILES64\LosungenQt"
RequestExecutionLevel admin

Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

Section "Installieren"
    SetOutPath "$INSTDIR"
    File /r "LosungenQt\*.*"
    
    ; Startmenü-Eintrag erstellen
    CreateDirectory "$SMPROGRAMS\LosungenQt"
    CreateShortCut "$SMPROGRAMS\LosungenQt\LosungenQt.lnk" "$INSTDIR\LosungenQt.exe"

    ; Deinstallationsdatei erstellen
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
    ; Dateien und Startmenü-Eintrag entfernen
    Delete "$INSTDIR\*.*"
    RMDir /r "$INSTDIR"
    Delete "$SMPROGRAMS\LosungenQt\LosungenQt.lnk"
    RMDir /r "$SMPROGRAMS\LosungenQt"
    Delete "$INSTDIR\Uninstall.exe"
SectionEnd
