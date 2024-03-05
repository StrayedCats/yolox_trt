# yolox_trt
YOLOX & TensorRT package

## TensorRTのインストール

TensorRTのインストール方法を説明します。

### 動作環境
- OS: Ubuntu 22.04
- Driver version: 545.29.06 (aptでインストール)
- CUDA: 12.3 (CUDA-Toolkitをaptでインストール)
- libcudnn8-dev
- ROS distro: Humble
- CPU: Intel CPU (x86_64)
- GPU: RTX3060Ti

TensorRTのバージョンは8以降であれば特に問題なく使用できると思いますが、NVIDIAドライバとCUDAのバージョンには注意してください。この関係で、上記の動作環境ではdpkg経由のインストールができていませんが、他のシステムでは可能かもしれません。

### インストール環境
- TensorRT: 8.6 GA

### TensorRTのダウンロード
[ダウンロードサイト](https://developer.nvidia.com/nvidia-tensorrt-8x-download)にアクセスして、`TensorRT 8.6 GA for Linux x86_64 and CUDA 12.0 and 12.1 TAR Package`をダウンロードします。

> なぜtar.gz？: CUDA12.2 + TensorRTの組み合わせはまだサポートされていないため、dpkgでインストールしません。

tar.gzを以下のようにに展開します。この中には、TensorRTのライブラリやツールが含まれています。

```bash
# 例: /usr/localが汚くなるので非推奨
tar -xvf TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-12.0.tar.gz
cd ~/Downloads/TensorRT-8.6.1.6
sudo cp -r ./lib/* /usr/local/lib/
sudo cp -r ./include/* /usr/local/include/

echo "export LD_LIBRARY_PATH=LD_LIBRARY_PATH=:/usr/local/lib" >> ~/.bashrc
```

### cuDNNのダウンロード

[cudnn-archive](https://developer.nvidia.com/rdp/cudnn-archive)から直近の.deb (x86) ダウンロードして、次のコマンドでインストール

```bash
# 例
sudo dpkg -i ./cudnn-local-repo-ubuntu2204-8.9.7.29_1.0-1_amd64.deb
sudo cp /var/cudnn-local-repo-ubuntu2204-8.9.7.29/cudnn-local-08A7D361-keyring.gpg /usr/share/keyrings/
sudo apt update
sudo apt install -y libcudnn8-dev
```

<br>

## モデルの変換&実行方法

YOLOXのモデルをTensorRTに変換する方法を説明します。

### モデルと画像のダウンロード

YOLOX-Tinyのモデルと犬の画像をダウンロードします。

```bash
wget https://github.com/Megvii-BaseDetection/YOLOX/releases/download/0.1.1rc0/yolox_tiny.onnx -O ${HOME}/yolox_tiny.onnx
wget https://raw.githubusercontent.com/AlexeyAB/darknet/25505164a3bd6235c75deaad325878ceda90249a/data/dog.jpg -O ${HOME}/dog.jpg
```

### ONNXモデルの変換

trtexecを使って、ONNXモデルをTensorRTの量子化モデルに変換します。

trtexecで生成されたバイナリは、他のデバイス・環境で実行することはできません。

```bash
trtexec --onnx=${HOME}/yolox_tiny.onnx --saveEngine=${HOME}/yolox_tiny.trt --fp16 --verbose --workspace=$((1<<16))
```

### TensorRTの量子化モデルの実行

ros2でビルド・実行できます。

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone https://github.com/StrayedCats/yolox_trt.git -b humble

# ビルド
cd ~/ros2_ws
colcon build
```

### 実行

```bash
source ~/ros2_ws/install/setup.bash
ros2 run yolox_trt yolox_trt_example ${HOME}/yolox_tiny.trt ${HOME}/dog.jpg
```

実行結果は、${HOME}/dog_out.jpgに保存されます。

### nodeでの実行
```bash
ros2 run detector2d_node detector2d_node_exec --ros-args -p load_target_plugin:=detector2d_plugins::YoloxTrt  -p yolox_trt_plugin.model_path:=/path/to/engine.trt -p yolox_trt_plugin.imshow_isshow:=false
```
