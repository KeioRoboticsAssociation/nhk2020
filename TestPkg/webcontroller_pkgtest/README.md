# ROS対応のロボットをWeb上で動かすGUIのパッケージ
---
- ラズパイマウス、PassRobot、TryRobotをWebからも操作できるパッケージ
- 基本的にbesshy8が編集します

## 使い方（準備）
---
- このパッケージはrosbridge,roslibjsを使うので、rosbridge_suiteパッケージが必要です。aptで入れましょう。
- PimouseMoniter.launchはラズパイマウスをGUIから動かすパッケージです。raspi_rosパッケージが必要なので、git cloneしてください
- [ラズパイマウスのリポジトリ](https://github.com/Besshy8/raspi_ros/tree/dev)
- カメラの画像をブラウザに表示したい場合は画像処理系のライブラリが必要です。以下のパッケージ名をインストールしてください。
- cv_bridge(ROSでopencvを使うパッケージ)
- cv_camera（cv_camera_nodeが入っているパッケージ)
- image_transport_plugins(よくわからん、必要っぽいので入れてください)
- 画像のメッセージを受け取ってブラウザに配信するパッケージのmjpeg_serverもGit経由でインストールしてください。

## 使い方（手順）
---
### 動作確認
1. Moniter.launchを立ち上げる。
2. 手元のブラウザで　http:// git cloneしてきたPCのIPアドレス:8000 にアクセス

### ラズパイマウスの動かし方
1. ラズパイマウスにカメラを接続
2. PimouseMoniter.launchを立ち上げる
3. 手元のブラウザで　http:// git cloneしてきたPCのIPアドレス:8000 にアクセス
4. raspimouseをクリック
5. モニターは左から、カメラ画像のリアルタイム表示、センサデータのリアルタイム表示、コントローラ（topic service配信）