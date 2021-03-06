IS_FULL_MAT = True
IS_CUT = True

if not IS_FULL_MAT:
    NUM_COL = 6
    NUM_ROW = 6
    TOTAL_NUM_NODES = 36
else:
    if IS_CUT:
        NUM_COL = 32
        NUM_ROW = 14
        TOTAL_NUM_NODES = 448
    else:
        NUM_COL = 32
        NUM_ROW = 57
        TOTAL_NUM_NODES = 1824

NUM_CYCLE = 8

PATIENT_ID = 1
VOUT_CONSTANT = 0.0008056641

FIT_CURVE_DEGREE = 4
FIT_COEFFICIENTS = [[]]
WEIGHTS = [0, 200, 250, 300, 500, 700, 1800, 2500]

VOUT_PER_WEIGHT = {
    0: [
        [
            235.68360472000003,
            313.34962396,
            485.92287418,
            315.76661626000003,
            369.10157968,
            452.08498198,
        ],
        [
            283.96973978,
            399.12599514,
            544.41408784,
            415.02443338,
            515.0879146,
            604.4092078200001,
        ],
        [
            485.33205384,
            551.18166628,
            564.4482684600001,
            387.09474458,
            636.95803746,
            683.63284432,
        ],
        [
            366.14747797999996,
            693.3008135199999,
            518.68654758,
            342.89064096000004,
            534.0478764200001,
            529.9121340400001,
        ],
        [
            368.40333746,
            485.60060854000005,
            375.06349402000006,
            300.13673272,
            434.19923895999995,
            515.94728964,
        ],
        [
            301.80177186000003,
            556.12307276,
            458.10060726,
            397.89064352,
            484.79494444000005,
            504.56057036000004,
        ],
    ],
    18: [
        [
            234.28712028,
            301.37208434,
            484.2041241,
            317.80763198,
            358.89650108,
            435.64943434,
        ],
        [
            281.60645842,
            381.6162287,
            538.7207281999999,
            414.6484568,
            498.22267944,
            573.3642844999999,
        ],
        [
            476.09377215999996,
            520.56643048,
            555.69338524,
            378.33986136000004,
            634.11135764,
            616.22561462,
        ],
        [
            371.89454856000003,
            662.73928866,
            480.33693642000003,
            329.19435126,
            517.29006314,
            501.06935925999994,
        ],
        [
            358.41310262,
            466.2109592,
            367.06056396,
            290.25391976,
            429.4189653,
            501.71389054,
        ],
        [
            318.5058742,
            549.4629162,
            465.99611544,
            399.17970607999996,
            494.5166245799999,
            512.2412347799999,
        ],
    ],
    30: [
        [
            256.79200414,
            333.8134921,
            522.01662586,
            359.48732142,
            382.36818186000005,
            470.02443594,
        ],
        [
            302.55372502000006,
            411.26466758,
            561.3867448799999,
            460.62502144,
            512.6709223,
            617.03127872,
        ],
        [
            499.83400764000004,
            568.2617452000001,
            576.64065184,
            420.39552738,
            663.5986637,
            652.587921,
        ],
        [
            401.4892765,
            697.49026684,
            497.25588252,
            359.11134484,
            542.96389246,
            534.1552982999999,
        ],
        [
            385.80568202000006,
            526.42092294,
            390.85451038,
            323.82325726,
            461.32326366000007,
            547.15334578,
        ],
        [
            344.93165668000006,
            569.92678434,
            492.79787450000003,
            426.30373078,
            529.21389182,
            551.3427991000001,
        ],
    ],
    45: [
        [
            242.93458162000005,
            320.70802274,
            507.89064864,
            345.0927895,
            365.07325918,
            455.03908368,
        ],
        [
            287.6220837,
            398.10548728,
            548.7646739800001,
            451.60158352,
            486.67482734000004,
            600.27346544,
        ],
        [
            485.70803042000006,
            546.88479108,
            532.86623574,
            399.23341702000005,
            611.92873942,
            623.10061494,
        ],
        [
            379.14552546000004,
            674.7705392199999,
            472.76369388,
            345.41505514000005,
            519.7607663800001,
            504.88283599999994,
        ],
        [
            363.83790756,
            520.5127195400001,
            378.55470512,
            312.8662255,
            440.80568458000005,
            531.84572788,
        ],
        [
            331.71876544,
            551.77248662,
            484.47267880000004,
            429.25783248,
            513.42287546,
            547.2070567200001,
        ],
    ],
    60: [
        [
            249.00391784,
            327.58302306,
            502.3047108800001,
            343.05177378,
            372.75392360000006,
            469.37990466,
        ],
        [
            286.65528678000004,
            393.75490114,
            537.4316656400001,
            437.52931724000007,
            474.64357678000005,
            587.81252736,
        ],
        [
            492.36818698,
            544.03811126,
            514.06740674,
            384.35548664000004,
            600.70315296,
            618.69631786,
        ],
        [
            377.05079880000005,
            675.2539376799999,
            464.86818569999997,
            341.38673464000004,
            518.68654758,
            511.3281488,
        ],
        [
            377.48048632,
            524.91701662,
            382.47560374,
            314.10157712000006,
            448.43263806000004,
            540.38576734,
        ],
        [
            340.15138301999997,
            558.9160416400001,
            488.82326494,
            427.32423864000003,
            516.10842246,
            562.56838556,
        ],
    ],
    75: [
        [
            238.90626112,
            323.12501504,
            495.4834215,
            344.34083634,
            371.51857198,
            462.82716998000006,
        ],
        [
            274.08692682000003,
            386.718768,
            515.51760212,
            439.83888765999995,
            477.16799096,
            576.5869409,
        ],
        [
            484.63381161999996,
            544.73635348,
            504.7217031800001,
            377.15822068,
            613.8623332599999,
            632.33889662,
        ],
        [
            363.13966534,
            668.32522642,
            462.34377152,
            339.29200798,
            521.5869383400001,
            513.10060982,
        ],
        [
            346.05958642,
            508.69631274000005,
            363.99904038000005,
            302.01661562000004,
            443.06154405999996,
            523.30568842,
        ],
        [
            326.77735896,
            556.23049464,
            473.51564704000003,
            425.33693386,
            522.6074462,
            556.76760404,
        ],
    ],
    90: [
        [
            247.66114434,
            331.23536698,
            505.2051016400001,
            341.3330237,
            378.1250176,
            471.68947508,
        ],
        [
            287.24610712000003,
            401.05958898000006,
            522.82228996,
            454.66310710000005,
            486.88967110000004,
            607.0947548199999,
        ],
        [
            492.69045262,
            556.82131498,
            499.88771858,
            381.29396306,
            625.1416306599999,
            634.00393576,
        ],
        [
            372.80763454000004,
            673.48147666,
            466.31838107999994,
            345.57618796,
            530.9863528400001,
            516.0547115200001,
        ],
        [
            365.71779046,
            522.66115714,
            364.53614977999996,
            307.60255338,
            445.74709106,
            518.2031491200001,
        ],
        [
            345.14650044,
            576.2109643200001,
            483.07619436000004,
            439.46291108,
            535.87404838,
            575.5127221,
        ],
    ],
    105: [
        [
            245.24415204,
            334.51173431999996,
            516.21584434,
            353.25685238,
            379.62892392000003,
            483.1299053,
        ],
        [
            285.4736461,
            403.20802657999997,
            531.14748566,
            448.37892712,
            489.41408528,
            614.50686454,
        ],
        [
            478.88674104,
            565.25393256,
            493.38869483999997,
            385.32228355999996,
            625.94729476,
            639.8584282200001,
        ],
        [
            372.91505642,
            679.1211253600001,
            462.93459186,
            346.9726724,
            538.77443914,
            522.01662586,
        ],
        [
            356.15724314000005,
            520.7275633,
            359.16505578000005,
            310.23438944,
            447.51955208000004,
            531.41604036,
        ],
        [
            340.20509396,
            572.5586204,
            481.84084274,
            436.50880937999995,
            534.58498582,
            575.8886986800001,
        ],
    ],
    120: [
        [
            241.21583153999998,
            326.72364802000004,
            506.27932044,
            336.23048439999997,
            372.4853689,
            480.28322548,
        ],
        [
            277.4170051,
            390.04884628,
            521.8017821,
            432.91017640000007,
            478.18849881999995,
            587.3291289,
        ],
        [
            481.46486616000004,
            543.3398690399999,
            481.19631146,
            367.11427490000005,
            616.49416932,
            628.1494433,
        ],
        [
            366.46974362000003,
            677.0263987000001,
            460.41017768000006,
            332.90040612,
            530.28811062,
            515.7861568200001,
        ],
        [
            351.69923512,
            499.56545294000006,
            349.01368812,
            290.36134164,
            435.75685622000003,
            517.71975066,
        ],
        [
            332.52442953999997,
            554.5654555,
            470.83010004,
            416.68947252000004,
            516.37697716,
            560.09768232,
        ],
    ],
    135: [
        [
            239.92676898000002,
            320.3857571,
            502.25099994,
            324.03810102,
            370.17579847999997,
            473.78420174000007,
        ],
        [
            277.36329416,
            386.66505706000004,
            507.35353924000003,
            431.45998102,
            472.76369388,
            585.8252225800001,
        ],
        [
            479.79982702,
            531.95314976,
            476.52345968000003,
            376.78224410000007,
            610.4248331,
            633.3594044800001,
        ],
        [
            360.07814176000005,
            650.49319434,
            449.88283344,
            333.75978116,
            515.94728964,
            506.54787514,
        ],
        [
            351.5381023,
            495.5371324400001,
            350.03419597999994,
            292.45606829999997,
            432.1045123,
            522.8760009,
        ],
        [
            336.66017192000004,
            552.36330696,
            472.06545165999995,
            423.83302754000005,
            527.1728761,
            557.9492447199999,
        ],
    ],
    150: [
        [
            239.71192522000004,
            325.27345264,
            507.8369377,
            321.83595247999995,
            369.26271249999996,
            478.24220976000004,
        ],
        [
            279.18946612,
            394.18458866,
            519.3847898,
            427.59279334,
            480.1758036,
            584.69729284,
        ],
        [
            470.61525628,
            532.4902591599999,
            488.50099930000005,
            367.75880618,
            611.8750284800001,
            631.103545,
        ],
        [
            366.0400561,
            682.3974927,
            474.42873302,
            338.05665636000003,
            524.91701662,
            522.98342278,
        ],
        [
            347.6709146200001,
            494.30178082,
            355.62013374,
            290.46876352,
            428.6133012,
            518.68654758,
        ],
        [
            341.49415652000005,
            563.8574481200001,
            488.71584306000005,
            423.08107438,
            529.53615746,
            572.71975322,
        ],
    ],
    165: [
        [
            245.1904411,
            325.32716358000005,
            502.62697651999997,
            325.38087452,
            380.86427554,
            480.06838172,
        ],
        [
            278.59864578,
            382.4218928,
            499.13576542,
            420.50294926,
            479.74611608000004,
            577.5000268800001,
        ],
        [
            471.52834226,
            528.73049336,
            467.39259988,
            382.20704903999996,
            604.7851843999999,
            633.73538106,
        ],
        [
            368.34962652,
            666.2842107,
            463.90138878,
            342.19239874,
            524.0039306399999,
            503.00295310000007,
        ],
        [
            348.20802402,
            481.35744428000004,
            348.5840006,
            294.22852932,
            422.43654310000005,
            526.0986573,
        ],
        [
            340.90333618,
            556.49904934,
            469.91701406000004,
            426.62599642,
            525.93752448,
            552.25588508,
        ],
    ],
    180: [
        [
            241.1621206,
            329.46290596,
            499.61916388,
            334.08204680000006,
            384.40919758,
            481.57228804000005,
        ],
        [
            272.5830205,
            389.02833842,
            500.2099842200001,
            426.57228548000006,
            482.64650684000003,
            582.4951443,
        ],
        [
            458.58400572000005,
            527.49514174,
            470.07814688,
            382.09962716,
            600.32717638,
            615.74221616,
        ],
        [
            359.0576339,
            645.65920974,
            471.5820532,
            340.95704712,
            534.58498582,
            499.18947636000007,
        ],
        [
            334.08204680000006,
            472.33400636,
            341.3330237,
            291.22071668,
            413.14455047999996,
            509.82424248,
        ],
        [
            337.25099226000003,
            560.58108078,
            473.13967046,
            425.9277542,
            535.55178274,
            547.63674424,
        ],
    ],
    195: [
        [
            243.04200350000002,
            331.93360920000003,
            512.939477,
            323.60841350000004,
            386.93361176,
            492.95900732,
        ],
        [
            278.97462235999996,
            393.59376832000004,
            516.86037562,
            420.66408208,
            492.36818698,
            602.2070592800001,
        ],
        [
            470.93752192,
            527.54885268,
            477.27541283999994,
            373.8281424,
            623.79885716,
            639.53616258,
        ],
        [
            359.91700894,
            678.26175032,
            477.00685814,
            337.73439072,
            541.1914314400001,
            518.1494381800001,
        ],
        [
            327.74415588,
            463.7939669,
            338.00294542000006,
            277.84669262,
            419.42873046,
            502.51955463999997,
        ],
        [
            339.82911738000007,
            559.3994401000001,
            481.67970992000005,
            420.82521490000005,
            551.45022098,
            573.74026108,
        ],
    ],
    210: [
        [
            240.46387837999998,
            326.66993707999995,
            494.89260116,
            304.05763133999994,
            386.50392424,
            485.8691632400001,
        ],
        [
            270.1660282,
            378.82325982,
            497.04103876,
            390.47853380000004,
            485.92287418,
            554.45803362,
        ],
        [
            469.91701406000004,
            512.02639102,
            458.63771666,
            360.34669646000003,
            611.6601847200001,
            628.47170894,
        ],
        [
            363.24708722,
            676.86526588,
            467.17775612,
            326.99220272,
            541.5136970799999,
            517.93459442,
        ],
        [
            325.2197417,
            468.89650620000003,
            337.51954696,
            275.5371222,
            413.89650364000005,
            505.31252352,
        ],
        [
            339.88282832,
            554.0283460999999,
            473.83791268000004,
            407.88087836,
            546.5088145,
            574.0088157800001,
        ],
    ],
}
