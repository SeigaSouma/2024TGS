xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 299;
 -201.26396;154.87205;-3.35627;,
 -201.26396;157.75326;-2.16278;,
 -14.31277;157.75326;-2.16278;,
 -14.31295;154.87200;-3.35627;,
 -201.26396;158.94701;0.71852;,
 -14.31277;158.94701;0.71852;,
 -201.26396;157.75326;3.59983;,
 -14.31277;157.75326;3.59983;,
 -201.26396;154.87205;4.79332;,
 -14.31295;154.87200;4.79332;,
 -201.26403;151.99069;3.59983;,
 -14.31295;151.99069;3.59983;,
 -201.26403;150.79718;0.71852;,
 -14.31295;150.79718;0.71852;,
 -201.26403;151.99069;-2.16278;,
 -14.31295;151.99069;-2.16278;,
 -201.26396;154.87205;-3.35627;,
 -14.31295;154.87200;-3.35627;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -201.26396;154.87205;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -14.31295;154.87200;0.71852;,
 -192.75209;150.12484;-0.04670;,
 -45.70607;150.12484;-0.04670;,
 -45.70607;-151.43007;-0.04670;,
 -192.75209;-151.43007;-0.04670;,
 -45.70607;150.12484;0.82575;,
 -45.70607;-151.43007;0.82575;,
 -192.75209;150.12484;0.82575;,
 -192.75209;-151.43007;0.82575;,
 -192.75209;150.12484;0.82575;,
 -45.70607;150.12484;0.82575;,
 -45.70607;150.12484;-0.04670;,
 -192.75209;150.12484;-0.04670;,
 -190.92731;152.44231;3.60369;,
 -190.92731;152.44231;3.06316;,
 -190.78004;152.21580;3.13554;,
 -190.92731;152.44231;3.60369;,
 -190.67228;152.04934;3.33343;,
 -190.92731;152.44231;3.60369;,
 -190.63283;151.98878;3.60369;,
 -190.92731;152.44231;3.60369;,
 -190.67228;152.04934;3.87395;,
 -190.92731;152.44231;3.60369;,
 -190.78004;152.21580;4.07169;,
 -190.92731;152.44231;3.60369;,
 -190.92731;152.44231;4.14430;,
 -190.92731;152.44231;3.60369;,
 -191.07431;152.66908;4.07169;,
 -190.92731;152.44231;3.60369;,
 -191.18212;152.83520;3.87395;,
 -190.92731;152.44231;3.60369;,
 -191.22160;152.89582;3.60369;,
 -190.92731;152.44231;3.60369;,
 -191.18212;152.83520;3.33343;,
 -190.92731;152.44231;3.60369;,
 -191.07431;152.66908;3.13554;,
 -190.92731;152.44231;3.60369;,
 -190.92731;152.44231;3.06316;,
 -47.81233;148.98020;3.60369;,
 -47.96961;148.76018;3.13554;,
 -47.81233;148.98020;3.06316;,
 -47.81233;148.98020;3.60369;,
 -48.08446;148.59901;3.33343;,
 -47.81233;148.98020;3.60369;,
 -48.12662;148.54019;3.60369;,
 -47.81233;148.98020;3.60369;,
 -48.08446;148.59901;3.87395;,
 -47.81233;148.98020;3.60369;,
 -47.96961;148.76018;4.07169;,
 -47.81233;148.98020;3.60369;,
 -47.81233;148.98020;4.14430;,
 -47.81233;148.98020;3.60369;,
 -47.65577;149.20032;4.07169;,
 -47.81233;148.98020;3.60369;,
 -47.54071;149.36151;3.87395;,
 -47.81233;148.98020;3.60369;,
 -47.49865;149.42045;3.60369;,
 -47.81233;148.98020;3.60369;,
 -47.54071;149.36151;3.33343;,
 -47.81233;148.98020;3.60369;,
 -47.65577;149.20032;3.13554;,
 -47.81233;148.98020;3.60369;,
 -47.81233;148.98020;3.06316;,
 -200.78495;154.82259;-3.27577;,
 -220.68535;154.82255;0.71852;,
 -200.78495;160.01299;-2.10586;,
 -220.68535;154.82255;0.71852;,
 -200.78495;162.16306;0.71852;,
 -220.68535;154.82255;0.71852;,
 -200.78495;160.01299;3.54291;,
 -220.68535;154.82255;0.71852;,
 -200.78495;154.82259;4.71282;,
 -220.68535;154.82255;0.71852;,
 -200.78495;149.63241;3.54291;,
 -220.68535;154.82255;0.71852;,
 -200.78495;147.48256;0.71852;,
 -220.68535;154.82255;0.71852;,
 -200.78495;149.63241;-2.10586;,
 -220.68535;154.82255;0.71852;,
 -200.78495;154.82259;-3.27577;,
 -200.78495;154.82259;0.71852;,
 -200.78495;154.82259;-3.27577;,
 -200.78495;160.01299;-2.10586;,
 -200.78495;162.16306;0.71852;,
 -200.78495;160.01299;3.54291;,
 -200.78495;154.82259;4.71282;,
 -200.78495;149.63241;3.54291;,
 -200.78495;147.48256;0.71852;,
 -200.78495;149.63241;-2.10586;,
 4.24209;170.81424;18.04672;,
 -3.74660;170.81424;18.45685;,
 -3.74660;145.02008;18.44140;,
 4.24209;145.02008;18.04672;,
 -3.74660;134.27237;11.61089;,
 4.24209;134.27237;11.21614;,
 -3.74660;46.60791;11.61089;,
 4.24209;46.60791;11.21614;,
 -3.74660;37.26312;19.80251;,
 -3.74660;10.02891;18.73579;,
 4.24209;10.02891;18.34103;,
 4.24209;37.26312;19.40774;,
 4.24209;6.95559;8.07245;,
 4.24209;10.02891;18.34103;,
 -3.74660;10.02891;18.73579;,
 -3.74660;6.95559;8.26979;,
 -3.74660;1.27442;-0.10434;,
 4.24209;1.27442;-0.10441;,
 4.24209;6.95559;8.07245;,
 4.24209;37.26312;-0.10441;,
 4.24209;37.26312;9.65171;,
 4.24209;37.26312;19.40774;,
 4.24209;10.02891;18.34103;,
 -3.74660;6.95559;8.26979;,
 -3.74660;10.02891;18.73579;,
 -3.74660;37.26312;19.80251;,
 -3.74660;37.26312;9.84904;,
 -3.74660;37.26312;-0.10434;,
 -3.74660;1.27442;-0.10434;,
 4.24209;46.60791;-0.10441;,
 4.24209;46.60791;5.55594;,
 4.24209;46.60791;11.21614;,
 -3.74660;46.60791;11.61089;,
 -3.74660;46.60791;5.75328;,
 -3.74660;46.60791;-0.10434;,
 4.24209;134.27237;-0.10441;,
 4.24209;134.27237;5.55594;,
 4.24209;134.27237;11.21614;,
 -3.74660;134.27237;11.61089;,
 -3.74660;134.27237;5.75328;,
 -3.74660;134.27237;-0.10434;,
 4.24209;145.02008;-0.10441;,
 4.24209;145.02008;8.97116;,
 4.24209;145.02008;18.04672;,
 -3.74660;145.02008;18.44140;,
 -3.74660;145.02008;9.16850;,
 -3.74660;145.02008;-0.10434;,
 4.24209;183.42399;-0.10441;,
 4.24209;177.11902;8.97116;,
 4.24209;170.81424;18.04672;,
 -3.74660;170.81424;18.45685;,
 -3.74660;177.11902;9.17622;,
 -3.74660;183.42399;-0.10434;,
 4.24209;177.11902;8.97116;,
 -3.74660;183.42399;-0.10434;,
 -3.74660;177.11902;9.17622;,
 -3.74660;170.81424;18.45685;,
 4.24209;170.81424;18.04672;,
 4.24209;145.02008;-18.25546;,
 -3.74660;145.02008;-18.65015;,
 -3.74660;170.81424;-18.66560;,
 4.24209;170.81424;-18.25546;,
 4.24209;134.27237;-11.42489;,
 -3.74660;134.27237;-11.81964;,
 4.24209;46.60791;-11.42489;,
 -3.74660;46.60791;-11.81964;,
 4.24209;37.26312;-19.61649;,
 4.24209;10.02891;-18.54978;,
 -3.74660;10.02891;-18.94446;,
 -3.74660;37.26312;-20.01126;,
 -3.74660;6.95559;-8.47854;,
 -3.74660;10.02891;-18.94446;,
 4.24209;10.02891;-18.54978;,
 4.24209;6.95559;-8.28120;,
 4.24209;37.26312;-9.86046;,
 4.24209;6.95559;-8.28120;,
 4.24209;10.02891;-18.54978;,
 4.24209;37.26312;-19.61649;,
 -3.74660;37.26312;-10.05779;,
 -3.74660;37.26312;-20.01126;,
 -3.74660;10.02891;-18.94446;,
 -3.74660;6.95559;-8.47854;,
 4.24209;46.60791;-5.76462;,
 4.24209;46.60791;-11.42489;,
 -3.74660;46.60791;-5.96203;,
 -3.74660;46.60791;-11.81964;,
 4.24209;134.27237;-5.76462;,
 4.24209;134.27237;-11.42489;,
 -3.74660;134.27237;-5.96203;,
 -3.74660;134.27237;-11.81964;,
 4.24209;145.02008;-9.17991;,
 4.24209;145.02008;-18.25546;,
 -3.74660;145.02008;-9.37724;,
 -3.74660;145.02008;-18.65015;,
 4.24209;177.11902;-9.17991;,
 4.24209;170.81424;-18.25546;,
 -3.74660;177.11902;-9.38496;,
 -3.74660;170.81424;-18.66560;,
 -3.74660;177.11902;-9.38496;,
 4.24209;177.11902;-9.17991;,
 4.24209;170.81424;-18.25546;,
 -3.74660;170.81424;-18.66560;,
 -3.26894;167.47788;-12.19193;,
 -3.26894;167.47788;-4.13403;,
 -3.26894;159.97738;-4.13403;,
 -3.26894;159.97738;-12.19193;,
 -3.26894;167.47788;3.92378;,
 -3.26894;159.97738;3.92378;,
 -3.26894;167.47788;11.98160;,
 -3.26894;159.97738;11.98160;,
 -3.26894;150.80799;-4.13403;,
 -3.26894;150.80799;-12.19193;,
 -3.26894;150.80799;3.92378;,
 -3.26894;150.80799;11.98160;,
 -3.26894;143.30436;-4.13403;,
 -3.26894;143.30436;-12.19193;,
 -3.26894;143.30436;3.92378;,
 -3.26894;143.30436;11.98160;,
 -9.26509;167.59658;4.45895;,
 -3.26894;167.47788;3.92378;,
 -3.26894;167.47788;-4.13403;,
 -9.26524;167.59658;-4.66889;,
 -22.69086;159.55584;-4.13403;,
 -22.69086;159.55584;4.02715;,
 -9.26524;167.59658;12.09261;,
 -3.26894;167.47788;11.98160;,
 -3.26894;167.47788;-12.19193;,
 -9.26524;167.59658;-12.30593;,
 -9.26524;167.59658;12.09261;,
 -9.26524;159.95674;12.09261;,
 -3.26894;159.97738;11.98160;,
 -9.26524;167.59658;-12.30593;,
 -3.26894;159.97738;-12.19193;,
 -9.26524;159.95674;-12.30593;,
 -22.69086;159.55584;4.02715;,
 -22.69086;151.36232;4.02715;,
 -9.26524;150.82888;12.09261;,
 -3.26894;150.80799;11.98160;,
 -3.26894;150.80799;-12.19193;,
 -9.26524;150.82888;-12.30593;,
 -22.69086;151.36232;-4.13403;,
 -22.69086;159.55584;-4.13403;,
 -9.26524;143.15799;12.09261;,
 -3.26894;143.30436;11.98160;,
 -3.26894;143.30436;-12.19193;,
 -9.26524;143.15799;-12.30593;,
 -9.26524;143.15799;12.09261;,
 -9.26509;143.15799;4.45895;,
 -3.26894;143.30436;3.92378;,
 -9.26524;143.15799;-12.30593;,
 -3.26894;143.30436;-4.13403;,
 -9.26524;143.15799;-4.66889;,
 -22.69086;151.36232;4.02715;,
 -22.69086;151.36232;-4.13403;,
 -9.26509;167.59658;4.45895;,
 -22.69086;159.55584;4.02715;,
 -9.26524;167.59658;-4.66889;,
 -9.26524;167.59658;-12.30593;,
 -22.69086;159.55584;-4.13403;,
 -9.26524;167.59658;12.09261;,
 -22.69086;159.55584;4.02715;,
 -9.26524;167.59658;-12.30593;,
 -9.26524;159.95674;-12.30593;,
 -9.26524;150.82888;12.09261;,
 -22.69086;151.36232;4.02715;,
 -9.26524;150.82888;-12.30593;,
 -9.26524;143.15799;-12.30593;,
 -22.69086;151.36232;-4.13403;,
 -9.26524;143.15799;12.09261;,
 -22.69086;151.36232;4.02715;,
 -9.26524;143.15799;-12.30593;,
 -9.26524;143.15799;-4.66889;,
 -22.69086;155.45899;-0.05340;,
 -22.69086;159.55584;4.02715;,
 -22.69086;159.55584;-4.13403;,
 -22.69086;151.36232;4.02715;,
 -22.69086;151.36232;-4.13403;;
 
 165;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,34,37,41;,
 4;42,43,44,45;,
 4;37,36,39,41;,
 3;46,47,48;,
 3;49,48,50;,
 3;51,50,52;,
 3;53,52,54;,
 3;55,54,56;,
 3;57,56,58;,
 3;59,58,60;,
 3;61,60,62;,
 3;63,62,64;,
 3;65,64,66;,
 3;67,66,68;,
 3;69,68,70;,
 3;71,72,73;,
 3;74,75,72;,
 3;76,77,75;,
 3;78,79,77;,
 3;80,81,79;,
 3;82,83,81;,
 3;84,85,83;,
 3;86,87,85;,
 3;88,89,87;,
 3;90,91,89;,
 3;92,93,91;,
 3;94,95,93;,
 3;96,97,98;,
 3;98,99,100;,
 3;100,101,102;,
 3;102,103,104;,
 3;104,105,106;,
 3;106,107,108;,
 3;108,109,110;,
 3;110,111,112;,
 3;113,114,115;,
 3;113,115,116;,
 3;113,116,117;,
 3;113,117,118;,
 3;113,118,119;,
 3;113,119,120;,
 3;113,120,121;,
 3;113,121,114;,
 4;122,123,124,125;,
 4;125,124,126,127;,
 4;127,126,128,129;,
 4;130,131,132,133;,
 4;130,133,129,128;,
 4;134,135,136,137;,
 4;134,137,138,139;,
 4;140,139,141,142;,
 4;140,142,143,144;,
 4;145,146,147,148;,
 4;145,148,149,150;,
 4;142,141,151,152;,
 4;142,152,153,143;,
 4;148,147,154,155;,
 4;148,155,156,149;,
 4;152,151,157,158;,
 4;152,158,159,153;,
 4;155,154,160,161;,
 4;155,161,162,156;,
 4;158,157,163,164;,
 4;158,164,165,159;,
 4;161,160,166,167;,
 4;161,167,168,162;,
 4;164,163,169,170;,
 4;164,170,171,165;,
 4;167,166,172,173;,
 4;167,173,174,168;,
 4;175,169,176,177;,
 4;175,177,178,179;,
 4;180,181,182,183;,
 4;184,185,181,180;,
 4;186,187,185,184;,
 4;188,189,190,191;,
 4;187,186,188,191;,
 4;192,193,194,195;,
 4;139,138,192,195;,
 4;196,141,139,197;,
 4;198,199,196,197;,
 4;200,201,202,203;,
 4;150,149,200,203;,
 4;204,151,141,196;,
 4;199,205,204,196;,
 4;206,207,201,200;,
 4;149,156,206,200;,
 4;208,157,151,204;,
 4;205,209,208,204;,
 4;210,211,207,206;,
 4;156,162,210,206;,
 4;212,163,157,208;,
 4;209,213,212,208;,
 4;214,215,211,210;,
 4;162,168,214,210;,
 4;216,169,163,212;,
 4;213,217,216,212;,
 4;218,219,215,214;,
 4;168,174,218,214;,
 4;220,176,169,221;,
 4;222,223,220,221;,
 4;224,225,226,227;,
 4;225,228,229,226;,
 4;228,230,231,229;,
 4;227,226,232,233;,
 4;226,229,234,232;,
 4;229,231,235,234;,
 4;233,232,236,237;,
 4;232,234,238,236;,
 4;234,235,239,238;,
 4;240,241,242,243;,
 4;240,243,244,245;,
 4;240,246,247,241;,
 4;243,242,248,249;,
 4;250,251,252,247;,
 4;253,248,254,255;,
 4;251,256,257,258;,
 4;251,258,259,252;,
 4;255,254,260,261;,
 4;255,261,262,263;,
 4;258,264,265,259;,
 4;261,260,266,267;,
 4;268,269,270,265;,
 4;271,266,272,273;,
 4;269,274,275,273;,
 4;269,273,272,270;,
 3;276,277,246;,
 3;278,279,280;,
 3;281,282,251;,
 3;283,284,263;,
 3;285,286,264;,
 3;287,288,289;,
 3;290,291,269;,
 3;292,293,275;,
 3;294,295,296;,
 3;294,297,295;,
 3;294,298,297;,
 3;294,296,298;;
 
 MeshMaterialList {
  5;
  165;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  4,
  1,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.787451;0.774902;0.800000;1.000000;;
   5.000000;
   0.210000;0.210000;0.210000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\arena\\IronTex001.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.360000;0.360000;0.360000;;
   TextureFilename {
    "data\\TEXTURE\\arena\\emblem000.png";
   }
  }
  Material {
   0.649412;0.649412;0.649412;1.000000;;
   5.000000;
   0.260000;0.260000;0.260000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\arena\\sabi.jpg";
   }
  }
  Material {
   0.649412;0.649412;0.649412;1.000000;;
   5.000000;
   0.260000;0.260000;0.260000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\arena\\sabi.jpg";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  121;
  -1.000000;0.000011;-0.000000;,
  0.000000;0.000006;-1.000000;,
  0.000000;0.707082;-0.707131;,
  0.000000;1.000000;0.000000;,
  0.000000;0.707082;0.707131;,
  0.000000;0.000006;1.000000;,
  -0.000000;-0.707104;0.707110;,
  0.000000;-1.000000;0.000000;,
  -0.000000;-0.707104;-0.707110;,
  1.000000;-0.000025;-0.000000;,
  -1.000000;0.000000;0.000000;,
  -1.000000;0.000013;0.000000;,
  -1.000000;0.000022;-0.000004;,
  -1.000000;0.000018;-0.000000;,
  -1.000000;0.000022;0.000004;,
  1.000000;-0.000030;-0.000000;,
  1.000000;-0.000051;0.000009;,
  1.000000;-0.000042;-0.000000;,
  1.000000;-0.000051;-0.000009;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.838758;-0.544505;0.000000;,
  -0.838723;-0.544559;-0.000353;,
  -0.838881;-0.544315;-0.000149;,
  -0.838706;-0.544584;-0.000000;,
  -0.838881;-0.544315;0.000149;,
  -0.838723;-0.544558;0.000353;,
  -0.838758;-0.544505;0.000000;,
  -0.839027;-0.544090;0.000104;,
  -0.838893;-0.544296;0.000322;,
  -0.838858;-0.544351;0.000000;,
  -0.838893;-0.544296;-0.000321;,
  -0.839027;-0.544090;-0.000104;,
  -0.838844;-0.544372;0.000000;,
  0.814155;-0.580648;0.000000;,
  0.814217;-0.580560;-0.000000;,
  0.813793;-0.581155;-0.000265;,
  0.813900;-0.581005;-0.000055;,
  0.813740;-0.581229;-0.000000;,
  0.813900;-0.581005;0.000055;,
  0.813793;-0.581155;0.000265;,
  0.814592;-0.580034;0.000314;,
  0.814348;-0.580377;0.000416;,
  0.814419;-0.580278;0.000000;,
  0.814348;-0.580377;-0.000415;,
  0.814592;-0.580034;-0.000314;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.196790;-0.000005;-0.980446;,
  -0.251433;0.519699;-0.816514;,
  -0.281618;0.763477;-0.581200;,
  -0.251433;0.519699;0.816514;,
  -0.196790;-0.000005;0.980446;,
  -0.251429;-0.519723;0.816499;,
  -0.281609;-0.763508;0.581164;,
  -0.251429;-0.519724;-0.816499;,
  -0.247219;0.283431;-0.926579;,
  1.000000;0.000000;0.000000;,
  -0.281618;0.763477;0.581200;,
  -0.247219;0.283431;0.926580;,
  -0.281609;-0.763508;-0.581164;,
  0.040709;0.465421;0.884153;,
  -1.000000;0.000000;0.000000;,
  0.047887;-0.279183;0.959043;,
  0.047392;-0.279039;0.959109;,
  0.046198;0.351867;0.934909;,
  0.049318;-0.039090;0.998018;,
  0.006937;-0.824379;0.565995;,
  1.000000;0.000000;0.000000;,
  0.046530;0.333501;0.941601;,
  0.008828;-0.902602;0.430385;,
  0.040709;0.465421;-0.884153;,
  0.047887;-0.279183;-0.959043;,
  0.047391;-0.279039;-0.959110;,
  0.046198;0.351867;-0.934910;,
  0.049312;-0.039092;-0.998018;,
  0.046527;0.333500;-0.941601;,
  0.008824;-0.902602;-0.430385;,
  -0.870002;0.493049;-0.000002;,
  -0.870002;0.493048;0.000001;,
  -0.870336;0.000000;0.492458;,
  -0.871762;0.000000;-0.489930;,
  -0.256994;0.966413;-0.000001;,
  -0.256995;0.966413;0.000001;,
  -0.256993;0.966413;-0.000005;,
  -0.256996;0.966412;0.000000;,
  -0.258269;0.000000;0.966073;,
  -0.260832;0.000000;-0.965384;,
  -0.258269;0.000000;0.966073;,
  -0.259075;-0.965857;-0.000002;,
  -0.259076;-0.965857;0.000001;,
  0.010502;-0.958700;0.284224;,
  0.014478;0.824145;0.566194;,
  0.007215;0.824205;0.566245;,
  0.010501;-0.958700;-0.284226;,
  0.006931;-0.824379;-0.565995;,
  0.014476;0.824145;-0.566194;,
  0.007209;0.824205;-0.566245;,
  0.019792;0.999804;0.000000;,
  0.019792;0.999804;-0.000000;,
  0.019792;0.999804;0.000000;,
  0.019792;0.999804;0.000000;,
  -0.258269;0.000000;0.966073;,
  0.018510;0.000000;0.999829;,
  -0.260832;0.000000;-0.965384;,
  0.019008;0.000000;-0.999819;,
  -0.514970;0.000000;0.857209;,
  0.018510;0.000000;0.999829;,
  -0.519937;0.000000;-0.854205;,
  0.018510;0.000000;0.999829;,
  -0.259074;-0.965857;-0.000005;,
  0.024404;-0.999702;0.000000;,
  0.024404;-0.999702;0.000000;,
  -0.259077;-0.965857;0.000000;,
  0.024404;-0.999702;-0.000000;,
  0.024404;-0.999702;-0.000000;,
  -0.521437;-0.853290;-0.000003;,
  -0.521439;-0.853288;0.000002;;
  165;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,10,11;,
  3;0,10,10;,
  3;0,10,10;,
  3;0,11,10;,
  3;0,12,11;,
  3;0,13,12;,
  3;0,14,13;,
  3;0,11,14;,
  3;9,15,16;,
  3;9,16,17;,
  3;9,17,18;,
  3;9,18,15;,
  3;9,15,19;,
  3;9,19,19;,
  3;9,19,19;,
  3;9,19,15;,
  4;20,20,20,20;,
  4;21,21,21,21;,
  4;22,22,22,22;,
  4;48,48,48,48;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  3;35,23,24;,
  3;35,24,25;,
  3;35,25,26;,
  3;35,26,27;,
  3;35,27,28;,
  3;35,28,29;,
  3;35,29,30;,
  3;35,30,31;,
  3;35,31,32;,
  3;35,32,33;,
  3;35,33,34;,
  3;35,34,23;,
  3;36,38,37;,
  3;36,39,38;,
  3;36,40,39;,
  3;36,41,40;,
  3;36,42,41;,
  3;36,37,42;,
  3;36,43,37;,
  3;36,44,43;,
  3;36,45,44;,
  3;36,46,45;,
  3;36,47,46;,
  3;36,37,47;,
  3;51,59,52;,
  3;52,59,53;,
  3;61,62,54;,
  3;54,62,55;,
  3;55,62,56;,
  3;56,57,57;,
  3;63,63,58;,
  3;58,59,51;,
  3;60,60,60;,
  3;60,60,60;,
  3;60,60,60;,
  3;60,60,60;,
  3;60,60,60;,
  3;60,60,60;,
  3;60,60,60;,
  3;60,60,60;,
  4;64,64,66,66;,
  4;66,66,67,67;,
  4;67,67,68,68;,
  4;72,69,69,72;,
  4;72,72,68,68;,
  4;73,94,94,73;,
  4;73,73,70,70;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;95,96,96,95;,
  4;95,95,64,64;,
  4;75,75,74,74;,
  4;76,76,75,75;,
  4;77,77,76,76;,
  4;79,78,78,79;,
  4;77,77,79,79;,
  4;80,97,97,80;,
  4;98,98,80,80;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;65,65,65,65;,
  4;65,65,65,65;,
  4;99,100,100,99;,
  4;74,74,99,99;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;71,71,71,71;,
  4;85,101,102,86;,
  4;85,86,82,81;,
  4;85,87,103,101;,
  4;86,102,104,88;,
  4;105,89,106,106;,
  4;107,108,108,90;,
  4;89,109,83,91;,
  4;89,91,110,106;,
  4;90,108,108,90;,
  4;90,90,84,111;,
  4;91,91,112,110;,
  4;90,108,108,90;,
  4;113,92,114,115;,
  4;116,117,118,93;,
  4;92,119,120,93;,
  4;92,93,118,114;,
  3;85,81,87;,
  3;86,88,82;,
  3;105,109,89;,
  3;107,90,111;,
  3;91,83,91;,
  3;90,90,84;,
  3;113,119,92;,
  3;116,93,120;,
  3;65,81,82;,
  3;65,83,81;,
  3;65,84,83;,
  3;65,82,84;;
 }
 MeshTextureCoords {
  299;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  -0.005010;-0.008880;,
  1.005010;-0.008880;,
  1.005010;1.001140;,
  -0.005010;1.001140;,
  1.005010;-0.008880;,
  1.005010;1.001140;,
  -0.005010;-0.008880;,
  -0.005010;1.001140;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.041670;1.000000;,
  0.000000;1.000000;,
  0.083330;1.000000;,
  0.125000;1.000000;,
  0.166670;1.000000;,
  0.208330;1.000000;,
  0.250000;1.000000;,
  0.291670;1.000000;,
  0.333330;1.000000;,
  0.375000;1.000000;,
  0.416670;1.000000;,
  0.458330;1.000000;,
  0.500000;1.000000;,
  0.541670;1.000000;,
  0.583330;1.000000;,
  0.625000;1.000000;,
  0.666670;1.000000;,
  0.708330;1.000000;,
  0.750000;1.000000;,
  0.791670;1.000000;,
  0.833330;1.000000;,
  0.875000;1.000000;,
  0.916670;1.000000;,
  0.958330;1.000000;,
  1.000000;1.000000;,
  0.041670;0.000000;,
  0.083330;0.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.166670;0.000000;,
  0.208330;0.000000;,
  0.250000;0.000000;,
  0.291670;0.000000;,
  0.333330;0.000000;,
  0.375000;0.000000;,
  0.416670;0.000000;,
  0.458330;0.000000;,
  0.500000;0.000000;,
  0.541670;0.000000;,
  0.583330;0.000000;,
  0.625000;0.000000;,
  0.666670;0.000000;,
  0.708330;0.000000;,
  0.750000;0.000000;,
  0.791670;0.000000;,
  0.833330;0.000000;,
  0.875000;0.000000;,
  0.916670;0.000000;,
  0.958330;0.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.000000;0.250000;,
  1.000000;0.500000;,
  0.000000;0.500000;,
  1.000000;0.750000;,
  0.000000;0.750000;,
  1.000000;0.875000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.875000;,
  0.375000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.750000;1.000000;,
  0.375000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.500000;1.000000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.500000;0.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.250000;0.000000;,
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.375000;0.000000;,
  0.500000;1.000000;,
  0.375000;1.000000;,
  0.250000;1.000000;,
  0.250000;0.000000;,
  0.000000;0.250000;,
  1.000000;0.250000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.750000;,
  1.000000;0.750000;,
  0.000000;0.875000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.875000;,
  0.375000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.625000;0.875000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.750000;0.875000;,
  0.375000;0.875000;,
  0.250000;0.875000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.375000;0.750000;,
  0.250000;0.750000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.375000;0.500000;,
  0.250000;0.500000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.375000;0.250000;,
  0.250000;0.250000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.375000;0.000000;,
  0.250000;0.000000;,
  0.375000;1.000000;,
  0.375000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;0.000000;,
  0.333330;0.000000;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  1.000000;0.000000;,
  1.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.666670;,
  0.333330;1.000000;,
  0.000000;1.000000;,
  0.666670;1.000000;,
  1.000000;1.000000;,
  0.333330;0.165830;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  0.666670;0.165830;,
  0.666670;1.000000;,
  0.333330;1.000000;,
  0.000000;0.165830;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.165830;,
  0.165830;0.000000;,
  0.165830;0.333330;,
  0.000000;0.333330;,
  0.834170;0.000000;,
  1.000000;0.333330;,
  0.834170;0.333330;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.165830;0.666670;,
  0.000000;0.666670;,
  1.000000;0.666670;,
  0.834170;0.666670;,
  0.000000;0.666670;,
  0.000000;0.333330;,
  0.165830;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.834170;1.000000;,
  0.000000;0.834170;,
  0.333330;0.834170;,
  0.333330;1.000000;,
  1.000000;0.834170;,
  0.666670;1.000000;,
  0.666670;0.834170;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  0.333330;0.582910;,
  0.166670;1.000000;,
  0.666670;0.582910;,
  0.833330;0.582910;,
  1.000000;1.000000;,
  0.582910;0.000000;,
  1.000000;0.166670;,
  0.417090;0.000000;,
  0.417090;0.166670;,
  0.582910;0.666670;,
  1.000000;0.833330;,
  0.417090;0.666670;,
  0.417090;0.833330;,
  0.000000;1.000000;,
  0.000000;0.417090;,
  0.166670;0.000000;,
  1.000000;0.417090;,
  0.833330;0.417090;,
  0.500000;0.500000;,
  0.333330;0.333330;,
  0.666670;0.333330;,
  0.333330;0.666670;,
  0.666670;0.666670;;
 }
}
