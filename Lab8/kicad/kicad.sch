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
L pspice:R R1
U 1 1 61854D18
P 3400 2950
F 0 "R1" H 3468 2996 50  0000 L CNN
F 1 "82.5" H 3468 2905 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 3400 2950 50  0001 C CNN
F 3 "~" H 3400 2950 50  0001 C CNN
	1    3400 2950
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R2
U 1 1 61857F5A
P 3800 3000
F 0 "R2" H 3868 3046 50  0000 L CNN
F 1 "10k" H 3868 2955 50  0000 L CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 3800 3000 50  0001 C CNN
F 3 "~" H 3800 3000 50  0001 C CNN
	1    3800 3000
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R3
U 1 1 6185965A
P 4300 3500
F 0 "R3" H 4232 3454 50  0000 R CNN
F 1 "680k" H 4232 3545 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 4300 3500 50  0001 C CNN
F 3 "~" H 4300 3500 50  0001 C CNN
	1    4300 3500
	-1   0    0    1   
$EndComp
$Comp
L pspice:R r4
U 1 1 6187932D
P 4700 4100
F 0 "r4" H 4632 4054 50  0000 R CNN
F 1 "1k" H 4632 4145 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 4700 4100 50  0001 C CNN
F 3 "~" H 4700 4100 50  0001 C CNN
	1    4700 4100
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R6
U 1 1 6187D634
P 5750 3350
F 0 "R6" H 5682 3304 50  0000 R CNN
F 1 "842k" H 5682 3395 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 5750 3350 50  0001 C CNN
F 3 "~" H 5750 3350 50  0001 C CNN
	1    5750 3350
	0    -1   -1   0   
$EndComp
$Comp
L pspice:R R9
U 1 1 61885D58
P 7450 3450
F 0 "R9" H 7382 3404 50  0000 R CNN
F 1 "100" H 7382 3495 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 7450 3450 50  0001 C CNN
F 3 "~" H 7450 3450 50  0001 C CNN
	1    7450 3450
	0    1    1    0   
$EndComp
$Comp
L pspice:DIODE D1
U 1 1 6188D84B
P 7700 3250
F 0 "D1" V 7654 3378 50  0000 L CNN
F 1 "DIODE" V 7745 3378 50  0000 L CNN
F 2 "Diode_SMD:D_0201_0603Metric" H 7700 3250 50  0001 C CNN
F 3 "~" H 7700 3250 50  0001 C CNN
	1    7700 3250
	0    1    1    0   
$EndComp
$Comp
L pspice:R 1k2
U 1 1 61881D78
P 6400 4200
F 0 "1k2" H 6332 4154 50  0000 R CNN
F 1 "R7" H 6332 4245 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric" H 6400 4200 50  0001 C CNN
F 3 "~" H 6400 4200 50  0001 C CNN
	1    6400 4200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male Vin1
U 1 1 61887F63
P 3200 2700
F 0 "Vin1" H 3308 2881 50  0000 C CNN
F 1 "Conn_01x01_Male" H 3308 2790 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Horizontal" H 3200 2700 50  0001 C CNN
F 3 "~" H 3200 2700 50  0001 C CNN
	1    3200 2700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male gnd1
U 1 1 6188AA28
P 5350 4550
F 0 "gnd1" V 5504 4462 50  0000 R CNN
F 1 "Conn_01x01_Male" V 5413 4462 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Horizontal" H 5350 4550 50  0001 C CNN
F 3 "~" H 5350 4550 50  0001 C CNN
	1    5350 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6000 3350 6150 3350
Wire Wire Line
	6150 3350 6150 3500
Wire Wire Line
	6950 3850 7100 3850
Wire Wire Line
	7100 3850 7100 3450
Wire Wire Line
	7100 3450 7050 3450
Wire Wire Line
	7200 3450 7100 3450
Connection ~ 7100 3450
Wire Wire Line
	5250 3750 5350 3750
Wire Wire Line
	5350 3750 5350 3350
Wire Wire Line
	5500 3350 5350 3350
$Comp
L pspice:R R5
U 1 1 61878E1F
P 5000 3750
F 0 "R5" H 4932 3704 50  0000 R CNN
F 1 "20k" H 4932 3795 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 5000 3750 50  0001 C CNN
F 3 "~" H 5000 3750 50  0001 C CNN
	1    5000 3750
	0    1    1    0   
$EndComp
Connection ~ 5350 3350
Wire Wire Line
	4700 3850 4700 3750
Wire Wire Line
	4700 3450 4750 3450
Wire Wire Line
	4750 3750 4700 3750
Connection ~ 4700 3750
Wire Wire Line
	4700 3750 4700 3450
Wire Wire Line
	4750 3250 4300 3250
Connection ~ 4300 3250
Wire Wire Line
	3400 2700 3800 2700
Wire Wire Line
	3800 2700 3800 2750
Connection ~ 3400 2700
Wire Wire Line
	3400 3250 3400 3200
Wire Wire Line
	3400 3250 2800 3250
Connection ~ 7700 3450
$Comp
L pspice:DIODE D2
U 1 1 6188CB8D
P 7700 3650
F 0 "D2" V 7654 3778 50  0000 L CNN
F 1 "DIODE" V 7745 3778 50  0000 L CNN
F 2 "Diode_SMD:D_0201_0603Metric" H 7700 3650 50  0001 C CNN
F 3 "~" H 7700 3650 50  0001 C CNN
	1    7700 3650
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x01_Male Vin2
U 1 1 618E2A83
P 7500 3050
F 0 "Vin2" H 7608 3231 50  0000 C CNN
F 1 "Conn_01x01_Male" H 7608 3140 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Horizontal" H 7500 3050 50  0001 C CNN
F 3 "~" H 7500 3050 50  0001 C CNN
	1    7500 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3850 7700 4450
Wire Wire Line
	7700 4450 6400 4450
Wire Wire Line
	4700 4350 5350 4350
Wire Wire Line
	6100 4350 6100 4450
Wire Wire Line
	6100 4450 6150 4450
Connection ~ 6400 4450
Wire Wire Line
	4700 4450 4700 4350
Connection ~ 4700 4350
Connection ~ 5350 4350
Wire Wire Line
	5350 4350 6100 4350
Wire Wire Line
	6150 4000 6150 4450
Connection ~ 6150 4450
Wire Wire Line
	6150 4450 6400 4450
Wire Wire Line
	4300 3750 4300 4450
Connection ~ 4300 4450
Wire Wire Line
	4300 4450 4700 4450
$Comp
L pspice:R R8
U 1 1 61881D72
P 6700 3850
F 0 "R8" H 6632 3804 50  0000 R CNN
F 1 "15k" H 6632 3895 50  0000 R CNN
F 2 "Resistor_SMD:R_0201_0603Metric_Pad0.64x0.40mm_HandSolder" H 6700 3850 50  0001 C CNN
F 3 "~" H 6700 3850 50  0001 C CNN
	1    6700 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 3950 6400 3850
Wire Wire Line
	6450 3850 6400 3850
$Comp
L Connector:Conn_01x01_Male OUT1
U 1 1 6193298D
P 8850 3450
F 0 "OUT1" H 8958 3631 50  0000 C CNN
F 1 "Conn_01x01_Male" H 8958 3540 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Horizontal" H 8850 3450 50  0001 C CNN
F 3 "~" H 8850 3450 50  0001 C CNN
	1    8850 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	7700 3450 8650 3450
Connection ~ 6400 3850
Connection ~ 6150 3350
Wire Wire Line
	6400 3850 6400 3550
Wire Wire Line
	6400 3550 6450 3550
Wire Wire Line
	6150 3350 6450 3350
$Comp
L pspice:CAP C2
U 1 1 61883FA3
P 6150 3750
F 0 "C2" V 5835 3750 50  0000 C CNN
F 1 "97.3uf" V 5926 3750 50  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_4x3" H 6150 3750 50  0001 C CNN
F 3 "~" H 6150 3750 50  0001 C CNN
	1    6150 3750
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:TLC277 U2
U 2 1 6188DFF6
P 6750 3450
F 0 "U2" H 6750 3817 50  0000 C CNN
F 1 "TLC277" H 6750 3726 50  0000 C CNN
F 2 "TLC277CP:DIP794W45P254L959H508Q8" H 6750 3450 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc272.pdf" H 6750 3450 50  0001 C CNN
	2    6750 3450
	1    0    0    -1  
$EndComp
$Comp
L pspice:CAP C1
U 1 1 618587C9
P 4050 3250
F 0 "C1" V 3735 3250 50  0000 C CNN
F 1 "97.3uf" V 3826 3250 50  0000 C CNN
F 2 "Capacitor_SMD:CP_Elec_3x5.4" H 4050 3250 50  0001 C CNN
F 3 "~" H 4050 3250 50  0001 C CNN
	1    4050 3250
	0    1    1    0   
$EndComp
$Comp
L Amplifier_Operational:TLC277 U2
U 1 1 6185AB20
P 5050 3350
F 0 "U2" H 5050 3717 50  0000 C CNN
F 1 "TLC277" H 5050 3626 50  0000 C CNN
F 2 "TLC277CP:DIP794W45P254L959H508Q8" H 5050 3350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc272.pdf" H 5050 3350 50  0001 C CNN
	1    5050 3350
	1    0    0    -1  
$EndComp
Connection ~ 3800 3250
Wire Wire Line
	3800 4450 2800 4450
Wire Wire Line
	3800 4450 4300 4450
Connection ~ 3800 4450
$Comp
L kicad-rescue:OPB745-OPB745 U1
U 1 1 61872E79
P 2800 3150
F 0 "U1" H 4028 2496 50  0000 L CNN
F 1 "OPB745" H 4028 2405 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2800 3350 50  0001 L CNN
F 3 "https://upverter.com/datasheet/4f8d3d5f99316b04551580b4d6f54087dceae820.pdf" H 2800 3450 50  0001 L CNN
F 4 "IC" H 2800 3550 50  0001 L CNN "category"
F 5 "SENSR OPTO TRANS 3.81MM REFL PCB" H 2800 3650 50  0001 L CNN "digikey description"
F 6 "365-1015-ND" H 2800 3750 50  0001 L CNN "digikey part number"
F 7 "yes" H 2800 3850 50  0001 L CNN "lead free"
F 8 "632e0746e169612c" H 2800 3950 50  0001 L CNN "library id"
F 9 "TT Electronics" H 2800 4050 50  0001 L CNN "manufacturer"
F 10 "828-OPB745" H 2800 4150 50  0001 L CNN "mouser part number"
F 11 "SENSOR_PTH_5MM08_17MM78" H 2800 4250 50  0001 L CNN "package"
F 12 "yes" H 2800 4350 50  0001 L CNN "rohs"
F 13 "+85°C" H 2800 4450 50  0001 L CNN "temperature range high"
F 14 "-40°C" H 2800 4550 50  0001 L CNN "temperature range low"
F 15 "15V" H 2800 4650 50  0001 L CNN "voltage"
F 16 "I" H 2800 3150 50  0001 C CNN "Spice_Primitive"
F 17 "dc 1 ac 1" H 2800 3150 50  0001 C CNN "Spice_Model"
F 18 "Y" H 2800 3150 50  0001 C CNN "Spice_Netlist_Enabled"
	1    2800 3150
	1    0    0    -1  
$EndComp
Text Notes 8100 7500 2    50   ~ 0
Heart Rate Monitor\n
Text Notes 7550 6800 2    50   ~ 0
Aubrey Tarmu\n11-12-21\n
$EndSCHEMATC
