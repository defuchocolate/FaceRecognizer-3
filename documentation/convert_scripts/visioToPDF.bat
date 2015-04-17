@echo off

REM übergabe parameter zählen:
SET /A ARGS_COUNT=0    
FOR %%A in (%*) DO SET /A ARGS_COUNT+=1    

if not %ARGS_COUNT%==1 (
   echo "too few arguments!"
   goto :end
)

REM filename ohne endung, mit endung wäre %~nx1:
set filenameWOext=%~n1
set filepath=%~pd1

set PDFFile="%filepath%%filenameWOext%.pdf"
set VisioFile="%filepath%%filenameWOext%.vsd"

echo %PDFFile%

if not exist %PDFFile% (
  echo "%PDFFile% does not exist!"
  goto convert
)

cscript //nologo convert_scripts\compareDate.vbs %PDFFile% %VisioFile%

if %errorlevel%==0 (
   goto :end
)

:convert
   echo "Executing OfficeToPDF.exe %VisioFile% %PDFFile% /hidden..."
   convert_scripts\OfficeToPDF.exe %VisioFile% %PDFFile% /hidden