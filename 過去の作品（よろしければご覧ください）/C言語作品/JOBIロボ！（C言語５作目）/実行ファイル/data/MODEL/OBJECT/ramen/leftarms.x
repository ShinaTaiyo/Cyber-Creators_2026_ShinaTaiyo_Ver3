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
 34;
 8.26732;13.20690;-4.79626;,
 9.58424;10.79420;-3.88392;,
 -6.89190;2.10308;-3.08520;,
 -8.20881;4.51578;-3.99754;,
 10.28780;9.69815;-1.29716;,
 -6.18832;1.00704;-0.49843;,
 9.96588;10.56077;1.44873;,
 -6.51024;1.86966;2.24746;,
 8.80706;12.87677;2.74525;,
 -7.66908;4.18566;3.54397;,
 7.49014;15.28947;1.83291;,
 -8.98599;6.59834;2.63163;,
 6.78656;16.38550;-0.75385;,
 -9.68955;7.69439;0.04487;,
 7.10847;15.52287;-3.49975;,
 -9.36763;6.83178;-2.70102;,
 8.26732;13.20690;-4.79626;,
 -8.20881;4.51578;-3.99754;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 8.53718;13.04183;-1.02551;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;,
 -7.93894;4.35072;-0.22678;;
 
 24;
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
 3;33,15,17;;
 
 MeshMaterialList {
  5;
  24;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.740392;0.015686;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.188235;0.163137;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.207059;0.260392;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.511373;0.511373;0.511373;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.517647;0.229020;0.100392;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  26;
  0.883675;0.466137;-0.042840;,
  -0.071318;0.043621;-0.996499;,
  0.276704;-0.593982;-0.755394;,
  0.462636;-0.883637;-0.071789;,
  0.377563;-0.655671;0.653867;,
  0.071318;-0.043621;0.996499;,
  -0.276705;0.593983;0.755393;,
  -0.462636;0.883637;0.071789;,
  -0.377562;0.655671;-0.653867;,
  -0.883675;-0.466136;0.042839;,
  0.883675;0.466137;-0.042837;,
  0.883674;0.466139;-0.042836;,
  0.883674;0.466138;-0.042837;,
  0.883675;0.466136;-0.042839;,
  0.883676;0.466134;-0.042842;,
  0.883675;0.466136;-0.042844;,
  0.883674;0.466138;-0.042843;,
  0.883673;0.466140;-0.042839;,
  -0.883677;-0.466134;0.042841;,
  -0.883675;-0.466136;0.042843;,
  -0.883674;-0.466138;0.042841;,
  -0.883674;-0.466138;0.042838;,
  -0.883675;-0.466137;0.042838;,
  -0.883675;-0.466137;0.042838;,
  -0.883675;-0.466137;0.042837;,
  -0.883676;-0.466135;0.042838;;
  24;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,10,11;,
  3;0,12,10;,
  3;0,13,12;,
  3;0,14,13;,
  3;0,15,14;,
  3;0,16,15;,
  3;0,17,16;,
  3;0,11,17;,
  3;9,18,19;,
  3;9,19,20;,
  3;9,20,21;,
  3;9,21,22;,
  3;9,22,23;,
  3;9,23,24;,
  3;9,24,25;,
  3;9,25,18;;
 }
 MeshTextureCoords {
  34;
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
  0.937500;1.000000;;
 }
}
