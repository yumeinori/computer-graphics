# computer-graphics
## 1.2D grid
實現一個基本的 2D 網格系統，為後續的繪製和渲染提供基礎。
## 2.Rasterization-Line
使用中點演算法實現線段的光柵化（Rasterization），同時還加入了反鋸齒技術，確保線段在渲染時不會出現鋸齒狀的邊緣，從而改善圖像品質。
## 3.Rasterizing polygons
scanline algorithms & color interpolation
將多邊形進行光柵化，並使用掃描線演算法和顏色插值實現多邊形的填充，以及在多邊形內部實現順暢的顏色過渡。
## 4.Arbitrary Rotation
實現對模型進行任意的旋轉變換，從而實現對場景中物體的3D空間操作。
## 5.DrawShadow
計算並繪製物體的陰影，這將增強場景的真實感和深度感。
## 6.Lighting and Subdivion
學習了如何實現燈光效果以及對模型進行細分，從而增強模型的細節和渲染效果。
## 7.Animation
實現基本的動畫效果，例如對模型進行平移、縮放和旋轉等變換，從而創建出場景中物體的運動效果。
## 8.final
OBJ parser  
Texture  
Animation  
Bounding Box  
解析 OBJ 格式的模型文件、實現紋理貼圖、進一步改進動畫效果，以及實現 Bounding Box 等技術。
### Render Mode
1.Point  
2.Line  
3.Face  
### Color Mode
1.Single  
2.Random  
