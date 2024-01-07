プラグインの実行方法

Terminal1
```bash
ros2 run detector2d_node detector2d_node_exec --ros-args -p load_target_plugin:=detector2d_plugins::YoloxTrt
```

Terminal2
```bash
ros2 run image_publisher image_publisher <image_path>
```