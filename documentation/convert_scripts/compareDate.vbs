Set objFS = CreateObject("Scripting.FileSystemObject")
Set objArgs = WScript.Arguments
strFile1 = objArgs(0)
strFile2 = objArgs(1)
Set objFile1 = objFS.GetFile(strFile1)
Set objFile2 = objFS.GetFile(strFile2)
If objFile1.DateLastModified < objFile2.DateLastModified Then
    'WScript.Echo ""&strFile1&" is older than "&strFile2
    WScript.quit 1
Else
    'WScript.Echo ""&strFile1&" is newer than "&strFile2
    WScript.quit 0
End If