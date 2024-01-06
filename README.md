# yolox_trt
YOLOX & TensorRT package

## TensorRTのインストール

TensorRTのインストール方法を説明します。

### 動作環境
- OS: Ubuntu 22.04
- Driver version: 535.129.03 (aptでインストール)
- CUDA: 12.2 (CUDA-Toolkitをaptでインストール、cuDNNは不要)
- ROS distro: Humble
- CPU: Intel CPU (x86_64)
- GPU: RTX3060Ti

### インストール環境
- TensorRT: 8.6

### TensorRTのダウンロード
[ダウンロードサイト](https://developer.nvidia.com/nvidia-tensorrt-8x-download)にアクセスして、`TensorRT 8.6 GA for Linux x86_64 and CUDA 12.0 and 12.1 TAR Package`をダウンロードします。

> なぜtar.gz？: CUDA12.2 + TensorRTの組み合わせはまだサポートされていないため、dpkgでインストールしません。

tar.gzをホームディレクトリに展開します。この中には、TensorRTのライブラリやツールが含まれています。


<br>

## モデルの変換&実行方法

YOLOXのモデルをTensorRTに変換する方法を説明します。

### モデルと画像のダウンロード

YOLOX-Tinyのモデルと画像をダウンロードします。

```bash
wget https://github.com/Megvii-BaseDetection/YOLOX/releases/download/0.1.1rc0/yolox_tiny.onnx -O ${HOME}/yolox_tiny.onnx
wget https://raw.githubusercontent.com/AlexeyAB/darknet/25505164a3bd6235c75deaad325878ceda90249a/data/dog.jpg -O ${HOME}/dog.jpg
```

### ONNXモデルの変換

trtexecを使って、ONNXモデルをTensorRTの量子化モデルに変換します。

trtexecで生成されたバイナリは、他のデバイス・環境で実行することはできません。

```bash
export TRT_ROOT=${HOME}/TensorRT-8.6.1.6/targets/x86_64-linux-gnu
$TRT_ROOT/bin/trtexec --onnx=${HOME}/yolox_tiny.onnx --saveEngine=${HOME}/yolox_tiny.trt --fp16 --verbose --workspace=$((1<<16))
```

### TensorRTの量子化モデルの実行

ros2でビルド・実行できます。

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone https://github.com/StrayedCats/yolox_trt.git -b humble

# ビルド
export TRT_ROOT=${HOME}/TensorRT-8.6.1.6/targets/x86_64-linux-gnu
export CPLUS_INCLUDE_PATH=${TRT_ROOT}/include:${CPLUS_INCLUDE_PATH}
export LD_LIBRARY_PATH=${TRT_ROOT}/lib:${LD_LIBRARY_PATH}

cd ~/ros2_ws
colcon build
```

### 実行

```bash
source ~/ros2_ws/install/setup.bash
ros2 run yolox_trt yolox_trt_example ${HOME}/yolox_tiny.trt ${HOME}/dog.jpg
```

実行結果は、${HOME}/dog_out.jpgに保存されます。

