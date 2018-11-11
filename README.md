ofxBoidSim
====

* ボイドモデルの描画の様子 
リボン状にしてみました。
![result](https://github.com/chakio/ofxBoidSim/blob/master/media/rebbon.gif) 
![result](https://github.com/chakio/ofxBoidSim/blob/master/media/boids.gif)
## Description

生命の群れなどを表現する際の有名な手法の一つとして、人口生命モデル
Boidsというモデルが提案されています。
このBoidsモデルを用いて鳥の群れを再現してみました。

## Environment
* Ubuntu16.04
* openFrameworks v0.10.0
* visualstudio code

## Boids model
基本的には以下の性質に基づき鳥は移動します。
<dl>
    <dt>Separation</dt>
    <dd>他の鳥とぶつからない</dd>
    <dt>Alignment</dt>
    <dd>概ね同じ方向に飛ぶように速度を合わせる</dd>
    <dt>Cohesion</dt>
    <dd>群れの中心方向に向かう</dd>
</dl>

## Originality
* 描画の都合上ある範囲内にとどまること
* 速度にノイズを載せ、単調な動きをを抑制すること
* 速度に依存させて描画時の色を変化させていること
