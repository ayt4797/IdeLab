EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "19 oct 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR01
U 1 1 5080AA99
P 11450 2300
F 0 "#PWR01" H 11450 2050 50  0001 C CNN
F 1 "GND" H 11450 2150 50  0000 C CNN
F 2 "" H 11450 2300 50  0000 C CNN
F 3 "" H 11450 2300 50  0000 C CNN
	1    11450 2300
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR02
U 1 1 5080AA9F
P 9150 750
F 0 "#PWR02" H 9150 600 50  0001 C CNN
F 1 "VCC" H 9150 900 50  0000 C CNN
F 2 "" H 9150 750 50  0000 C CNN
F 3 "" H 9150 750 50  0000 C CNN
	1    9150 750 
	1    0    0    -1  
$EndComp
$Comp
L boosterpack:Ti_Booster_40_J4 J4
U 1 1 5080DC12
P 8450 2400
F 0 "J4" H 8400 3050 60  0000 C CNN
F 1 "TI_BOOSTER_40_J4" H 8450 1750 60  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" V 8900 2400 60  0001 C CNN
F 3 "" H 8450 2400 60  0001 C CNN
	1    8450 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61854500
P 3400 2700
F 0 "#PWR?" H 3400 2550 50  0001 C CNN
F 1 "+5V" H 3415 2873 50  0000 C CNN
F 2 "" H 3400 2700 50  0001 C CNN
F 3 "" H 3400 2700 50  0001 C CNN
	1    3400 2700
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R1
U 1 1 61854D18
P 3400 2950
F 0 "R1" H 3468 2996 50  0000 L CNN
F 1 "R" H 3468 2905 50  0000 L CNN
F 2 "" H 3400 2950 50  0001 C CNN
F 3 "~" H 3400 2950 50  0001 C CNN
	1    3400 2950
	1    0    0    -1  
$EndComp
$Comp
L pspice:DIODE D?
U 1 1 618555B4
P 3400 3400
F 0 "D?" V 3354 3528 50  0000 L CNN
F 1 "DIODE" V 3445 3528 50  0000 L CNN
F 2 "" H 3400 3400 50  0001 C CNN
F 3 "~" H 3400 3400 50  0001 C CNN
	1    3400 3400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61856C44
P 3400 3600
F 0 "#PWR?" H 3400 3350 50  0001 C CNN
F 1 "GND" H 3405 3427 50  0000 C CNN
F 2 "" H 3400 3600 50  0001 C CNN
F 3 "" H 3400 3600 50  0001 C CNN
	1    3400 3600
	1    0    0    -1  
$EndComp
Wire Bus Line
	3400 2700 3800 2700
Wire Bus Line
	3800 2700 3800 2750
$Comp
L pspice:R R2
U 1 1 61857F5A
P 3800 3000
F 0 "R2" H 3868 3046 50  0000 L CNN
F 1 "R" H 3868 2955 50  0000 L CNN
F 2 "" H 3800 3000 50  0001 C CNN
F 3 "~" H 3800 3000 50  0001 C CNN
	1    3800 3000
	1    0    0    -1  
$EndComp
$Comp
L pspice:CAP C1
U 1 1 618587C9
P 4050 3250
F 0 "C1" V 3735 3250 50  0000 C CNN
F 1 "97.3uf" V 3826 3250 50  0000 C CNN
F 2 "" H 4050 3250 50  0001 C CNN
F 3 "~" H 4050 3250 50  0001 C CNN
	1    4050 3250
	0    1    1    0   
$EndComp
$Comp
L pspice:R R3
U 1 1 6185965A
P 4300 3500
F 0 "R3" H 4232 3454 50  0000 R CNN
F 1 "680k" H 4232 3545 50  0000 R CNN
F 2 "" H 4300 3500 50  0001 C CNN
F 3 "~" H 4300 3500 50  0001 C CNN
	1    4300 3500
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6185A224
P 4300 3750
F 0 "#PWR?" H 4300 3500 50  0001 C CNN
F 1 "GND" H 4305 3577 50  0000 C CNN
F 2 "" H 4300 3750 50  0001 C CNN
F 3 "" H 4300 3750 50  0001 C CNN
	1    4300 3750
	1    0    0    -1  
$EndComp
Wire Bus Line
	4300 3250 4750 3250
$Comp
L Amplifier_Operational:TLC277 U?
U 1 1 6185AB20
P 5050 3350
F 0 "U?" H 5050 3717 50  0000 C CNN
F 1 "TLC277" H 5050 3626 50  0000 C CNN
F 2 "" H 5050 3350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc272.pdf" H 5050 3350 50  0001 C CNN
	1    5050 3350
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R5
U 1 1 61878E1F
P 5000 3750
F 0 "R5" H 4932 3704 50  0000 R CNN
F 1 "20k" H 4932 3795 50  0000 R CNN
F 2 "" H 5000 3750 50  0001 C CNN
F 3 "~" H 5000 3750 50  0001 C CNN
	1    5000 3750
	0    1    1    0   
$EndComp
$Comp
L pspice:R 1k
U 1 1 6187932D
P 4700 4100
F 0 "1k" H 4632 4054 50  0000 R CNN
F 1 "R4" H 4632 4145 50  0000 R CNN
F 2 "" H 4700 4100 50  0001 C CNN
F 3 "~" H 4700 4100 50  0001 C CNN
	1    4700 4100
	1    0    0    -1  
$EndComp
Wire Bus Line
	4750 3450 4700 3450
Wire Bus Line
	4700 3450 4700 3750
Wire Bus Line
	4700 3750 4750 3750
Wire Bus Line
	4700 3750 4700 3850
Connection ~ 4700 3750
$Comp
L power:GND #PWR?
U 1 1 6187C7DD
P 4700 4350
F 0 "#PWR?" H 4700 4100 50  0001 C CNN
F 1 "GND" H 4705 4177 50  0000 C CNN
F 2 "" H 4700 4350 50  0001 C CNN
F 3 "" H 4700 4350 50  0001 C CNN
	1    4700 4350
	1    0    0    -1  
$EndComp
Wire Bus Line
	5250 3750 5350 3750
Wire Bus Line
	5350 3750 5350 3350
Wire Bus Line
	5350 3350 5500 3350
$Comp
L pspice:R R6
U 1 1 6187D634
P 5750 3350
F 0 "R6" H 5682 3304 50  0000 R CNN
F 1 "842k" H 5682 3395 50  0000 R CNN
F 2 "" H 5750 3350 50  0001 C CNN
F 3 "~" H 5750 3350 50  0001 C CNN
	1    5750 3350
	0    -1   -1   0   
$EndComp
Wire Bus Line
	6000 3350 6150 3350
$Comp
L Amplifier_Operational:TLC277 U?
U 1 1 61881D6C
P 6750 3450
F 0 "U?" H 6750 3817 50  0000 C CNN
F 1 "TLC277" H 6750 3726 50  0000 C CNN
F 2 "" H 6750 3450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc272.pdf" H 6750 3450 50  0001 C CNN
	1    6750 3450
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R8
U 1 1 61881D72
P 6700 3850
F 0 "R8" H 6632 3804 50  0000 R CNN
F 1 "15k" H 6632 3895 50  0000 R CNN
F 2 "" H 6700 3850 50  0001 C CNN
F 3 "~" H 6700 3850 50  0001 C CNN
	1    6700 3850
	0    1    1    0   
$EndComp
$Comp
L pspice:R 1k
U 1 1 61881D78
P 6400 4200
F 0 "1k" H 6332 4154 50  0000 R CNN
F 1 "R7" H 6332 4245 50  0000 R CNN
F 2 "" H 6400 4200 50  0001 C CNN
F 3 "~" H 6400 4200 50  0001 C CNN
	1    6400 4200
	1    0    0    -1  
$EndComp
Wire Bus Line
	6450 3550 6400 3550
Wire Bus Line
	6400 3550 6400 3850
Wire Bus Line
	6400 3850 6450 3850
Wire Bus Line
	6400 3850 6400 3950
Connection ~ 6400 3850
$Comp
L power:GND #PWR?
U 1 1 61881D83
P 6400 4450
F 0 "#PWR?" H 6400 4200 50  0001 C CNN
F 1 "GND" H 6405 4277 50  0000 C CNN
F 2 "" H 6400 4450 50  0001 C CNN
F 3 "" H 6400 4450 50  0001 C CNN
	1    6400 4450
	1    0    0    -1  
$EndComp
Wire Bus Line
	6950 3850 7050 3850
Wire Bus Line
	7050 3850 7050 3450
Wire Bus Line
	7050 3450 7200 3450
Wire Bus Line
	6150 3350 6150 3500
Connection ~ 6150 3350
Wire Bus Line
	6150 3350 6450 3350
$Comp
L pspice:CAP C2
U 1 1 61883FA3
P 6150 3750
F 0 "C2" V 5835 3750 50  0000 C CNN
F 1 "97.3uf" V 5926 3750 50  0000 C CNN
F 2 "" H 6150 3750 50  0001 C CNN
F 3 "~" H 6150 3750 50  0001 C CNN
	1    6150 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61884CBC
P 6150 4000
F 0 "#PWR?" H 6150 3750 50  0001 C CNN
F 1 "GND" H 6155 3827 50  0000 C CNN
F 2 "" H 6150 4000 50  0001 C CNN
F 3 "" H 6150 4000 50  0001 C CNN
	1    6150 4000
	1    0    0    -1  
$EndComp
$EndSCHEMATC
