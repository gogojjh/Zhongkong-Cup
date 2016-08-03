@echo off
echo.
echo Use method svm.
for %%i in (test\*) do (
..\bin\object_categorize svm "%%i" train result
)
echo.
echo.
echo Use method match.
for %%i in (test\*) do (
..\bin\object_categorize match "%%i" train result
)