ofxBoidSim
====

ボイドモデルの描画
## Description


生命の群れなどを表現する際の有名な手法の一つとして、人口生命モデル
Boidsというモデルが提案されています。
このBoidsモデルを用いて鳥の群れを再現してみました。

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