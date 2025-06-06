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
 36;
 -0.08025;1.99143;1.49952;,
 7.20563;1.12668;0.91338;,
 7.09535;0.17325;1.30938;,
 -0.25813;0.45340;2.13832;,
 6.98510;-0.78020;0.91338;,
 -0.43600;-1.08463;1.49952;,
 6.93942;-1.17515;-0.04262;,
 -0.50970;-1.72170;-0.04262;,
 6.98510;-0.78020;-0.99862;,
 -0.43600;-1.08463;-1.58477;,
 7.09535;0.17325;-1.39462;,
 -0.25813;0.45340;-2.22355;,
 7.20563;1.12668;-0.99862;,
 -0.08025;1.99143;-1.58477;,
 7.25133;1.52163;-0.04262;,
 -0.00655;2.62850;-0.04262;,
 7.20563;1.12668;0.91338;,
 -0.08025;1.99143;1.49952;,
 -0.25813;0.45340;2.13832;,
 -0.25813;0.45340;-0.04262;,
 -0.08025;1.99143;1.49952;,
 -0.43600;-1.08463;1.49952;,
 -0.50970;-1.72170;-0.04262;,
 -0.43600;-1.08463;-1.58477;,
 -0.25813;0.45340;-2.22355;,
 -0.08025;1.99143;-1.58477;,
 -0.00655;2.62850;-0.04262;,
 7.20563;1.12668;0.91338;,
 7.09535;0.17325;-0.04262;,
 7.09535;0.17325;1.30938;,
 6.98510;-0.78020;0.91338;,
 6.93942;-1.17515;-0.04262;,
 6.98510;-0.78020;-0.99862;,
 7.09535;0.17325;-1.39462;,
 7.20563;1.12668;-0.99862;,
 7.25133;1.52163;-0.04262;;
 
 24;
 4;3,2,1,0;,
 4;5,4,2,3;,
 4;7,6,4,5;,
 4;9,8,6,7;,
 4;11,10,8,9;,
 4;13,12,10,11;,
 4;15,14,12,13;,
 4;17,16,14,15;,
 3;20,19,18;,
 3;18,19,21;,
 3;21,19,22;,
 3;22,19,23;,
 3;23,19,24;,
 3;24,19,25;,
 3;25,19,26;,
 3;26,19,20;,
 3;29,28,27;,
 3;30,28,29;,
 3;31,28,30;,
 3;32,28,31;,
 3;33,28,32;,
 3;34,28,33;,
 3;35,28,34;,
 3;27,28,35;;
 
 MeshMaterialList {
  6;
  24;
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
   0.213600;0.213600;0.213600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.179200;0.106400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.304000;0.304000;0.304000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.276000;0.109600;0.068800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  26;
  0.139192;0.693835;0.706554;,
  0.111574;-0.011647;0.993688;,
  0.084351;-0.709864;0.699270;,
  0.073176;-0.997319;-0.000000;,
  0.084350;-0.709861;-0.699273;,
  0.111571;-0.011646;-0.993688;,
  0.139191;0.693833;-0.706557;,
  0.150766;0.988569;0.000000;,
  -0.993378;0.114891;0.000000;,
  0.993379;-0.114886;0.000000;,
  -0.993379;0.114886;0.000000;,
  -0.993378;0.114891;-0.000005;,
  -0.993378;0.114891;0.000005;,
  -0.993378;0.114895;0.000000;,
  -0.993378;0.114891;-0.000005;,
  -0.993379;0.114886;0.000000;,
  -0.993378;0.114891;0.000005;,
  -0.993378;0.114895;0.000000;,
  0.993377;-0.114903;0.000007;,
  0.993379;-0.114882;0.000000;,
  0.993381;-0.114869;-0.000002;,
  0.993380;-0.114871;0.000000;,
  0.993381;-0.114869;0.000002;,
  0.993379;-0.114882;0.000000;,
  0.993377;-0.114903;-0.000007;,
  0.993376;-0.114910;0.000000;;
  24;
  4;1,1,0,0;,
  4;2,2,1,1;,
  4;3,3,2,2;,
  4;4,4,3,3;,
  4;5,5,4,4;,
  4;6,6,5,5;,
  4;7,7,6,6;,
  4;0,0,7,7;,
  3;11,8,10;,
  3;10,8,12;,
  3;12,8,13;,
  3;13,8,14;,
  3;14,8,15;,
  3;15,8,16;,
  3;16,8,17;,
  3;17,8,11;,
  3;19,9,18;,
  3;20,9,19;,
  3;21,9,20;,
  3;22,9,21;,
  3;23,9,22;,
  3;24,9,23;,
  3;25,9,24;,
  3;18,9,25;;
 }
 MeshTextureCoords {
  36;
  0.375000;0.312500;,
  0.375000;0.688440;,
  0.406250;0.688440;,
  0.406250;0.312500;,
  0.437500;0.688440;,
  0.437500;0.312500;,
  0.468750;0.688440;,
  0.468750;0.312500;,
  0.500000;0.688440;,
  0.500000;0.312500;,
  0.531250;0.688440;,
  0.531250;0.312500;,
  0.562500;0.688440;,
  0.562500;0.312500;,
  0.593750;0.688440;,
  0.593750;0.312500;,
  0.625000;0.688440;,
  0.625000;0.312500;,
  0.500000;0.000000;,
  0.500000;0.150000;,
  0.610490;0.045770;,
  0.389520;0.045770;,
  0.343750;0.156250;,
  0.389520;0.266730;,
  0.500000;0.312500;,
  0.610490;0.266730;,
  0.656250;0.156250;,
  0.610490;0.954240;,
  0.500000;0.837500;,
  0.500000;1.000000;,
  0.389520;0.954240;,
  0.343750;0.843750;,
  0.389520;0.733260;,
  0.500000;0.687500;,
  0.610490;0.733260;,
  0.656250;0.843750;;
 }
}
