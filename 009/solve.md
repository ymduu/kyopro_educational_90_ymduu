# 考えたこと  
原点を全探索して、各点を原点からのベクトルに変換。
変換後の点を偏角ソートして、片方を全探索、もう片方を二分探索でN^2logN。

実装時に、
今回欲しいのは偏角だけであるので、偏角のみ保持してソートしたらよいこと
180度を超える角について、 360 から引けばよいこと
あたりがあいまいだったかも