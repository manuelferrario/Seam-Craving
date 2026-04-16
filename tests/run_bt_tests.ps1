param(
    [string]$ExePath = "",
    [string]$CompilerPath = ""
)

$ErrorActionPreference = "Stop"
$projectRoot = Split-Path -Parent $PSScriptRoot
$casesDir = Join-Path $PSScriptRoot "fb"
$algoritmo = "bt"
$wingetBin = 'C:\Users\manue\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin'
$runtimeBin = ""
if (-not [string]::IsNullOrWhiteSpace($CompilerPath) -and (Test-Path $CompilerPath)) {
    $runtimeBin = Split-Path -Parent $CompilerPath
} elseif (Test-Path $wingetBin) {
    $runtimeBin = $wingetBin
}
if (-not [string]::IsNullOrWhiteSpace($runtimeBin)) {
    $env:PATH = $runtimeBin + ";" + $env:PATH
}



if ([string]::IsNullOrWhiteSpace($ExePath)) {
    $ExePath = Join-Path $PSScriptRoot "seam_test.exe"
}

if (-not (Test-Path $ExePath)) {
    if ([string]::IsNullOrWhiteSpace($CompilerPath)) {
        $gpp = Get-Command g++ -ErrorAction SilentlyContinue
        if ($gpp) {
            $CompilerPath = $gpp.Source
        } else {
            $wingetGpp = 'C:\Users\manue\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\g++.exe'
            if (Test-Path $wingetGpp) { $CompilerPath = $wingetGpp }
        }
    }

    if ([string]::IsNullOrWhiteSpace($CompilerPath) -or -not (Test-Path $CompilerPath)) {
        Write-Host "No se encontro compilador C++ (g++)." -ForegroundColor Yellow
        exit 1
    }

    Push-Location $projectRoot
    try {
        $compileArgs = @(
            "-std=c++17",
            "-O2",
            "-o", $ExePath,
            (Join-Path $projectRoot 'tests\seam_test_main.cpp'),
            (Join-Path $projectRoot 'source\FuerzaBruta.cpp'),
            (Join-Path $projectRoot 'source\Backtracking.cpp')
        )
        & $CompilerPath @compileArgs
        if ($LASTEXITCODE -ne 0) {
            Write-Host "Fallo la compilacion del ejecutable de tests." -ForegroundColor Red
            exit 1
        }
    }
    finally {
        Pop-Location
    }
}

function Read-Matrix([string]$path) {
    $lines = Get-Content -Path $path | Where-Object { $_.Trim().Length -gt 0 }
    $header = $lines[0].Trim() -split ' +'
    $rows = [int]$header[0]
    $cols = [int]$header[1]

    $matrix = @()
    for ($r = 0; $r -lt $rows; $r++) {
        $vals = ($lines[$r + 1].Trim() -split ' +') | ForEach-Object { [double]$_ }
        if ($vals.Count -ne $cols) { throw "Formato invalido en $path (fila $r)" }
        $matrix += ,$vals
    }
    return ,$matrix
}

function Read-Expected([string]$path) {
    $data = @{}
    foreach ($line in Get-Content -Path $path) {
        if ($line -match '^[ ]*([^=]+)=(.*)$') {
            $data[$matches[1].Trim()] = $matches[2].Trim()
        }
    }

    $seam = @()
    if ($data.ContainsKey('seam') -and $data['seam'].Length -gt 0) {
        $seam = $data['seam'] -split ' +' | ForEach-Object { [int]$_ }
    }
    $energy = [double]$data['energy']

    return @{ seam = $seam; energy = $energy }
}

function Parse-Program-Output([string[]]$lines) {
    $seam = @()
    $energy = $null

    foreach ($line in $lines) {
        if ($line -match '^seam=(.*)$') {
            $raw = $matches[1].Trim()
            if ($raw.Length -gt 0) {
                $seam = $raw -split ' +' | ForEach-Object { [int]$_ }
            }
        }
        if ($line -match '^energy=(.*)$') {
            $energy = [double]$matches[1].Trim()
        }
    }

    if ($null -eq $energy) {
        throw "Salida invalida del ejecutable (falta energy=...)."
    }

    return @{ seam = $seam; energy = $energy }
}

$caseFiles = Get-ChildItem -Path $casesDir -Filter '*.txt' |
    Where-Object { $_.Name -notlike '*.expected.txt' } |
    Sort-Object Name

$passed = 0
$failed = 0

Push-Location $projectRoot
try {
    foreach ($case in $caseFiles) {
        $expectedPath = Join-Path $casesDir ($case.BaseName + '.expected.txt')
        if (-not (Test-Path $expectedPath)) {
            Write-Host "[FAIL] $($case.Name): falta expected" -ForegroundColor Red
            $failed++
            continue
        }

        $matrix = Read-Matrix $case.FullName
        $expected = Read-Expected $expectedPath

        $rawOutput = & $ExePath --numerico $case.FullName --algoritmo $algoritmo 2>&1
        if ($LASTEXITCODE -ne 0) {
            Write-Host "[FAIL] $($case.Name): el programa devolvio codigo $LASTEXITCODE" -ForegroundColor Red
            $rawOutput | ForEach-Object { Write-Host "       $_" }
            $failed++
            continue
        }

        try {
            $actual = Parse-Program-Output $rawOutput
        } catch {
            Write-Host "[FAIL] $($case.Name): $($_.Exception.Message)" -ForegroundColor Red
            $rawOutput | ForEach-Object { Write-Host "       $_" }
            $failed++
            continue
        }

        $sameSeam = ($actual.seam.Count -eq $expected.seam.Count)
        if ($sameSeam) {
            for ($i = 0; $i -lt $actual.seam.Count; $i++) {
                if ($actual.seam[$i] -ne $expected.seam[$i]) {
                    $sameSeam = $false
                    break
                }
            }
        }

        $sameEnergy = [Math]::Abs($actual.energy - $expected.energy) -lt 1e-9

        if ($sameSeam -and $sameEnergy) {
            Write-Host "[OK]   $($case.Name) -> seam=[$(($actual.seam -join ' '))], energia=$($actual.energy)" -ForegroundColor Green
            $passed++
        } else {
            Write-Host "[FAIL] $($case.Name)" -ForegroundColor Red
            Write-Host "       esperado seam=[$(($expected.seam -join ' '))], energia=$($expected.energy)"
            Write-Host "       obtenido seam=[$(($actual.seam -join ' '))], energia=$($actual.energy)"
            $failed++
        }
    }
}
finally {
    Pop-Location
}

Write-Host "\nResumen: $passed OK, $failed FAIL"
if ($failed -gt 0) { exit 1 }
exit 0



