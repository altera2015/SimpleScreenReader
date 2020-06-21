; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Simple Screen Reader"
#define MyAppVersion "2.0"
#define MyAppPublisher "Promixis, LLC"
#define MyAppURL "http://www.simplescreenreader.com"
#define MyAppExeName "SimpleReader.exe"

#define ApplicationVersion GetFileVersion("Release\SimpleReader.exe")

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{895E6B92-8869-4B32-B83D-9E481AAAAEB1}
AppName={#MyAppName}
AppVerName={#MyAppName} {#ApplicationVersion}
VersionInfoVersion={#ApplicationVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=installer
Compression=lzma
SolidCompression=yes
OutputBaseFilename=SimpleScreenReader-{#ApplicationVersion}
;SignTool=kSign

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "Release\SimpleReader.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "filters.json"; DestDir: "{app}"; Flags: onlyifdoesntexist
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Registry]
Root: HKCU; Subkey: "Software\Promixis\SimpleReader\InstallData"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"
Root: HKCU; Subkey: "Software\Promixis\SimpleReader"; ValueType: string; ValueName: "filterLocation"; ValueData: "{app}\filters.json"


[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

