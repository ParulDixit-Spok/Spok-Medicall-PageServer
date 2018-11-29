#This script updates vc project files in VS 2008 
#It is called from command line as a pre-build event in order to take
#in Jenkins version variables and apply them to the project so that 
#all of the assemlby versions match the product version
#Current as of 7/26/13:

#parameter passed in from command line
param([string]$projectPath)

#Quotes are needed to ensure the full project path is used.
#However, if a path does not need quotes, the quotes will also be passed in.
#This line removes such a case.
$projectPath = $projectPath -replace '"', ""


#Get the Version Resource file name so that we can modicy it later
$GetFile = Get-ChildItem ("{0}\*.rc" -f $projectPath) | select BaseName,Extension
$VersionFile = $GetFile.Basename + $GetFile.Extension

#Check if the Version Environment Variables exist, if they do, use that value, otherwise, use default values
If (Test-Path env:MAJOR_VERSION)
{
	$MAJOR = (Get-Item env:MAJOR_VERSION).value
}
else
{
	$MAJOR = "1"
}
If (Test-Path env:MINOR_VERSION)
{
	$MINOR = (Get-Item env:MINOR_VERSION).value
}
else
{
	$MINOR = "0"
	}
If (Test-Path env:REVISION)
{
	$REVISION = (Get-Item env:REVISION).value
	}
else
{
	$REVISION = "0"
}
If (Test-Path env:BUILD_NUMBER)
{
	$BUILD = (Get-Item env:BUILD_NUMBER).value
}
else
{
	$BUILD = "0"
}


#Updated how sections are replaced to provide an easy means of updating this section.
$ReplaceList = @{
"(?-i)FILEVERSION(.*)" = "FILEVERSION $MAJOR,$MINOR,$REVISION,$BUILD"
'(?-i)FileVersion"(.*)' = "FileVersion"", ""$MAJOR, $MINOR, $REVISION, $BUILD"""
"(?-i)PRODUCTVERSION(.*)" = "PRODUCTVERSION $MAJOR,$MINOR,$REVISION,$BUILD"
'(?-i)ProductVersion"(.*)' = "ProductVersion"", ""$MAJOR, $MINOR, $REVISION, $BUILD"""
}

#Create a variable to hold the ResourceFile reference.
#This is important as it provides a single location to change how paths are used.
$resourceFile = "{0}\{1}" -f $projectPath, $VersionFile

#Updated Versioning Script to Get all the content and close the file in one step before working on replacing data.
#This section will Read data from Resource file
#Iterate line-by-line and replace any matches to the keys of "ReplaceList"
#Saves the data back to the resource file.
(Get-Content ($resourceFile)) |
  Foreach-Object {
    $line = $_
    $ReplaceList.GetEnumerator() | ForEach-Object {
        if ($line -match $_.Key) { $line = $line -replace $_.Key, $_.Value }
    }
    $line
  } | Set-Content ($resourceFile)
# SIG # Begin signature block
# MIIVHwYJKoZIhvcNAQcCoIIVEDCCFQwCAQExCzAJBgUrDgMCGgUAMGkGCisGAQQB
# gjcCAQSgWzBZMDQGCisGAQQBgjcCAR4wJgIDAQAABBAfzDtgWUsITrck0sYpfvNR
# AgEAAgEAAgEAAgEAAgEAMCEwCQYFKw4DAhoFAAQU8GRV/qeCOLchGvO7Fsd39vRO
# A3qggg/fMIIEmTCCA4GgAwIBAgIPFojwOSVeY45pFDkH5jMLMA0GCSqGSIb3DQEB
# BQUAMIGVMQswCQYDVQQGEwJVUzELMAkGA1UECBMCVVQxFzAVBgNVBAcTDlNhbHQg
# TGFrZSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxITAfBgNV
# BAsTGGh0dHA6Ly93d3cudXNlcnRydXN0LmNvbTEdMBsGA1UEAxMUVVROLVVTRVJG
# aXJzdC1PYmplY3QwHhcNMTUxMjMxMDAwMDAwWhcNMTkwNzA5MTg0MDM2WjCBhDEL
# MAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE
# BxMHU2FsZm9yZDEaMBgGA1UEChMRQ09NT0RPIENBIExpbWl0ZWQxKjAoBgNVBAMT
# IUNPTU9ETyBTSEEtMSBUaW1lIFN0YW1waW5nIFNpZ25lcjCCASIwDQYJKoZIhvcN
# AQEBBQADggEPADCCAQoCggEBAOnpPd/XNwjJHjiyUlNCbSLxscQGBGue/YJ0UEN9
# xqC7H075AnEmse9D2IOMSPznD5d6muuc3qajDjscRBh1jnilF2n+SRik4rtcTv6O
# KlR6UPDV9syR55l51955lNeWM/4Og74iv2MWLKPdKBuvPavql9LxvwQQ5z1IRf0f
# aGXBf1mZacAiMQxibqdcZQEhsGPEIhgn7ub80gA9Ry6ouIZWXQTcExclbhzfRA8V
# zbfbpVd2Qm8AaIKZ0uPB3vCLlFdM7AiQIiHOIiuYDELmQpOUmJPv/QbZP7xbm1Q8
# ILHuatZHesWrgOkwmt7xpD9VTQoJNIp1KdJprZcPUL/4ygkCAwEAAaOB9DCB8TAf
# BgNVHSMEGDAWgBTa7WR0FJwUPKvdmam9WyhNizzJ2DAdBgNVHQ4EFgQUjmstM2v0
# M6eTsxOapeAK9xI1aogwDgYDVR0PAQH/BAQDAgbAMAwGA1UdEwEB/wQCMAAwFgYD
# VR0lAQH/BAwwCgYIKwYBBQUHAwgwQgYDVR0fBDswOTA3oDWgM4YxaHR0cDovL2Ny
# bC51c2VydHJ1c3QuY29tL1VUTi1VU0VSRmlyc3QtT2JqZWN0LmNybDA1BggrBgEF
# BQcBAQQpMCcwJQYIKwYBBQUHMAGGGWh0dHA6Ly9vY3NwLnVzZXJ0cnVzdC5jb20w
# DQYJKoZIhvcNAQEFBQADggEBALozJEBAjHzbWJ+zYJiy9cAx/usfblD2CuDk5oGt
# Joei3/2z2vRz8wD7KRuJGxU+22tSkyvErDmB1zxnV5o5NuAoCJrjOU+biQl/e8Vh
# f1mJMiUKaq4aPvCiJ6i2w7iH9xYESEE9XNjsn00gMQTZZaHtzWkHUxY93TYCCojr
# QOUGMAu4Fkvc77xVCf/GPhIudrPczkLv+XZX4bcKBUCYWJpdcRaTcYxlgepv84n3
# +3OttOe/2Y5vqgtPJfO44dXddZhogfiqwNGAwsTEOYnB9smebNd0+dmX+E/CmgrN
# Xo/4GengpZ/E8JIh5i15Jcki+cPwOoRXrToW9GOUEB1d0MYwggUwMIIEGKADAgEC
# AhAj0cKvXltydo0QX4kmsfFLMA0GCSqGSIb3DQEBBQUAMIG0MQswCQYDVQQGEwJV
# UzEXMBUGA1UEChMOVmVyaVNpZ24sIEluYy4xHzAdBgNVBAsTFlZlcmlTaWduIFRy
# dXN0IE5ldHdvcmsxOzA5BgNVBAsTMlRlcm1zIG9mIHVzZSBhdCBodHRwczovL3d3
# dy52ZXJpc2lnbi5jb20vcnBhIChjKTEwMS4wLAYDVQQDEyVWZXJpU2lnbiBDbGFz
# cyAzIENvZGUgU2lnbmluZyAyMDEwIENBMB4XDTE0MDgyNjAwMDAwMFoXDTE3MDgx
# ODIzNTk1OVowYjELMAkGA1UEBhMCVVMxEjAQBgNVBAgTCU1pbm5lc290YTEVMBMG
# A1UEBxMMRWRlbiBQcmFpcmllMRMwEQYDVQQKFApTcG9rLCBJbmMuMRMwEQYDVQQD
# FApTcG9rLCBJbmMuMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAr5/2
# xwDa6E7FsND+HTNCNn4v8JDRK4ha3+s/UuJxQBGREF17S2zboucJRm9Fu1jSctEK
# SOOPCPOBnOdTrQ73AwMH1nMoDPWWJGAbsAQN9pB5GXYCVD8SQbdYaelEgc4/40s0
# 8uXixB9pmO3BvlpDgmh1HnVQVSY48AZv/5Vr9SwG4UaHJUy1foSz7LPB+4+6kk1A
# vtrTdyTMdpQX+zyIqzqqWKcsfMK+XOTmtIjsGBJoIXXxjhkqFCJDG1qU5sDL3c3D
# Pae1+t+fbmKVwgW+9vbWeuD+CNtPyx10Bx9W+vfHI8h1t4AInx9Z7a7X+e08dHJt
# 2CXucPes0E9nj3WIzwIDAQABo4IBjTCCAYkwCQYDVR0TBAIwADAOBgNVHQ8BAf8E
# BAMCB4AwKwYDVR0fBCQwIjAgoB6gHIYaaHR0cDovL3NmLnN5bWNiLmNvbS9zZi5j
# cmwwZgYDVR0gBF8wXTBbBgtghkgBhvhFAQcXAzBMMCMGCCsGAQUFBwIBFhdodHRw
# czovL2Quc3ltY2IuY29tL2NwczAlBggrBgEFBQcCAjAZFhdodHRwczovL2Quc3lt
# Y2IuY29tL3JwYTATBgNVHSUEDDAKBggrBgEFBQcDAzBXBggrBgEFBQcBAQRLMEkw
# HwYIKwYBBQUHMAGGE2h0dHA6Ly9zZi5zeW1jZC5jb20wJgYIKwYBBQUHMAKGGmh0
# dHA6Ly9zZi5zeW1jYi5jb20vc2YuY3J0MB8GA1UdIwQYMBaAFM+Zqep7JvRLyY6P
# 1/AFJu/j0qedMB0GA1UdDgQWBBSyW6ifOvVMF08D+SHtDFFhx9ITDzARBglghkgB
# hvhCAQEEBAMCBBAwFgYKKwYBBAGCNwIBGwQIMAYBAQABAf8wDQYJKoZIhvcNAQEF
# BQADggEBAAqk7nlC7Fg+knGr2g3lJM9BfwAnWfy4QAphuyDLPEOZeQW9sviIay52
# VAmKl5b/YMgm4jEXr0gSgkS6zHePeJrjCHr5P7TNtl6oMtusqA/p3tWGbvkvBzxv
# 939KF82uNG/iCudLD0ZH4sKS+1wBMMe7wx0gCXeKii9l15Vmd7SmmESCGNo1X+Uj
# +zLgYDk3wXDcgZXjG2ukM1PuGFqpU4Yjqn77/sZ4DL7Q39bu0HP5bCp4o4/5kVNx
# QfcnivenslxMNz3vbnjPTAfh3+91Mhz/Of1gLd/nv9TWxSoHnWHFV2tDTGnu8kF/
# DKw5sV4IRE5cNxBXaFFW2WvQjX5IuuMwggYKMIIE8qADAgECAhBSAOWqJVb8Gobt
# lsnUSzPHMA0GCSqGSIb3DQEBBQUAMIHKMQswCQYDVQQGEwJVUzEXMBUGA1UEChMO
# VmVyaVNpZ24sIEluYy4xHzAdBgNVBAsTFlZlcmlTaWduIFRydXN0IE5ldHdvcmsx
# OjA4BgNVBAsTMShjKSAyMDA2IFZlcmlTaWduLCBJbmMuIC0gRm9yIGF1dGhvcml6
# ZWQgdXNlIG9ubHkxRTBDBgNVBAMTPFZlcmlTaWduIENsYXNzIDMgUHVibGljIFBy
# aW1hcnkgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkgLSBHNTAeFw0xMDAyMDgwMDAw
# MDBaFw0yMDAyMDcyMzU5NTlaMIG0MQswCQYDVQQGEwJVUzEXMBUGA1UEChMOVmVy
# aVNpZ24sIEluYy4xHzAdBgNVBAsTFlZlcmlTaWduIFRydXN0IE5ldHdvcmsxOzA5
# BgNVBAsTMlRlcm1zIG9mIHVzZSBhdCBodHRwczovL3d3dy52ZXJpc2lnbi5jb20v
# cnBhIChjKTEwMS4wLAYDVQQDEyVWZXJpU2lnbiBDbGFzcyAzIENvZGUgU2lnbmlu
# ZyAyMDEwIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA9SNLXqXX
# irsy6dRX9+/kxyZ+rRmY/qidfZT2NmsQ13WBMH8EaH/LK3UezR0IjN9plKc3o5x7
# gOCZ4e43TV/OOxTuhtTQ9Sc1vCULOKeMY50Xowilq7D7zWpigkzVIdob2fHjhDuK
# Kk+FW5ABT8mndhB/JwN8vq5+fcHd+QW8G0icaefApDw8QQA+35blxeSUcdZVAccA
# JkpAPLWhJqkMp22AjpAle8+/PxzrL5b65Yd3xrVWsno7VDBTG99iNP8e0fRakyiF
# 5UwXTn5b/aSTmX/fze+kde/vFfZH5/gZctguNBqmtKdMfr27Tww9V/Ew1qY2jtaA
# dtcZLqXNfjQtiQIDAQABo4IB/jCCAfowEgYDVR0TAQH/BAgwBgEB/wIBADBwBgNV
# HSAEaTBnMGUGC2CGSAGG+EUBBxcDMFYwKAYIKwYBBQUHAgEWHGh0dHBzOi8vd3d3
# LnZlcmlzaWduLmNvbS9jcHMwKgYIKwYBBQUHAgIwHhocaHR0cHM6Ly93d3cudmVy
# aXNpZ24uY29tL3JwYTAOBgNVHQ8BAf8EBAMCAQYwbQYIKwYBBQUHAQwEYTBfoV2g
# WzBZMFcwVRYJaW1hZ2UvZ2lmMCEwHzAHBgUrDgMCGgQUj+XTGoasjY5rw8+AatRI
# GCx7GS4wJRYjaHR0cDovL2xvZ28udmVyaXNpZ24uY29tL3ZzbG9nby5naWYwNAYD
# VR0fBC0wKzApoCegJYYjaHR0cDovL2NybC52ZXJpc2lnbi5jb20vcGNhMy1nNS5j
# cmwwNAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC52ZXJp
# c2lnbi5jb20wHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMDMCgGA1UdEQQh
# MB+kHTAbMRkwFwYDVQQDExBWZXJpU2lnbk1QS0ktMi04MB0GA1UdDgQWBBTPmanq
# eyb0S8mOj9fwBSbv49KnnTAfBgNVHSMEGDAWgBR/02Wnwt3su/AwCfNDOfoCrzMx
# MzANBgkqhkiG9w0BAQUFAAOCAQEAViLmNKTEYctIuQGtVqhkD9mMkcS7zAzlrXqg
# In/fRzhKLWzRf3EafOxwqbHwT+QPDFP6FV7+dJhJJIWBJhyRFEewTGOMu6E01MZF
# 6A2FJnMD0KmMZG3ccZLmRQVgFVlROfxYFGv+1KTteWsIDEFy5zciBgm+I+k/RJoe
# 6WGdzLGQXPw90o2sQj1lNtS0PUAoj5sQzyMmzEsgy5AfXYxMNMo82OU31m+lIL00
# 6ybZrg3nxZr3obQhkTNvhuhYuyV8dA5Y/nUbYz/OMXybjxuWnsVTdoRbnK2R+qzt
# k7pdyCFTwoJTY68SDVCHERs9VFKWiiycPZIaCJoFLseTpUiR0zGCBKowggSmAgEB
# MIHJMIG0MQswCQYDVQQGEwJVUzEXMBUGA1UEChMOVmVyaVNpZ24sIEluYy4xHzAd
# BgNVBAsTFlZlcmlTaWduIFRydXN0IE5ldHdvcmsxOzA5BgNVBAsTMlRlcm1zIG9m
# IHVzZSBhdCBodHRwczovL3d3dy52ZXJpc2lnbi5jb20vcnBhIChjKTEwMS4wLAYD
# VQQDEyVWZXJpU2lnbiBDbGFzcyAzIENvZGUgU2lnbmluZyAyMDEwIENBAhAj0cKv
# Xltydo0QX4kmsfFLMAkGBSsOAwIaBQCgcDAQBgorBgEEAYI3AgEMMQIwADAZBgkq
# hkiG9w0BCQMxDAYKKwYBBAGCNwIBBDAcBgorBgEEAYI3AgELMQ4wDAYKKwYBBAGC
# NwIBFTAjBgkqhkiG9w0BCQQxFgQUx7Ivq0hDa0g8BWXyJOz+PZ7306UwDQYJKoZI
# hvcNAQEBBQAEggEAW4zGnUl888W6mk6QQnthZJJIJDtEPXOC28nB049BmCx2g0Ry
# W38BU3AbWWIdY7dBehj918el/d2DfLKWGb3sWglMeNkt7O6GXTkNtD9rXV2msW/W
# El7ABzpCVJI4PFQ/P2kBv84uZhDRkNaweeMAwEIbFw7tu8bi+XGTJZU+zhwdES8Q
# mhL5F5nfnzDgjiBz77PTBFDVcN1jdQQUAUWWajUhcMjWBKpz+bMxCTnOCmteiY/9
# MrvRngXymmqU0XzW5uot91oTBrNfou1ONqQ0DwW/wa4f0Bu+fQPc2WACxqQZoLW4
# hGVfTj/9gk25RgaCgaXoUMpO/TMCCLzGRUVBR6GCAkMwggI/BgkqhkiG9w0BCQYx
# ggIwMIICLAIBATCBqTCBlTELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAlVUMRcwFQYD
# VQQHEw5TYWx0IExha2UgQ2l0eTEeMBwGA1UEChMVVGhlIFVTRVJUUlVTVCBOZXR3
# b3JrMSEwHwYDVQQLExhodHRwOi8vd3d3LnVzZXJ0cnVzdC5jb20xHTAbBgNVBAMT
# FFVUTi1VU0VSRmlyc3QtT2JqZWN0Ag8WiPA5JV5jjmkUOQfmMwswCQYFKw4DAhoF
# AKBdMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTE3
# MDIxNDE1MzQyN1owIwYJKoZIhvcNAQkEMRYEFOcHtAdI+JzMZWV0zzRHTyNUbiQm
# MA0GCSqGSIb3DQEBAQUABIIBAIdwvCrivZC5rhF4V33SWi5t6n/eJ+T41/noOysG
# js2wwkFV0ZrKIyrM7X/FoDW+BbvSgv4SuWrNCYYitE2RnqANSAbQI838DXkHt8hR
# 53S8inZuPVO4HfPq8ZN5RZ051/uq5H9aFwY8+J3WeLatuxfm41TaYUBA073dNWmt
# 2bcLB1ZLdqTQ03pivX8sfh60JOYZ7Onz5w/MPIEP4Q9NUtZbps9mCALHFFYfk5IC
# 9ft5qyJSGM72RQh/04TcgQnz9VrVphJjmRVn6qdnWntaozsQgZKAmHN+oHf3LCwI
# dgyNPBoQRi6yi96NHYhaeRmcmgOvLI/29HXJfDTFlu7Qi4M=
# SIG # End signature block
