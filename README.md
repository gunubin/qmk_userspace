# qmk_userspace (gunubin)

QMK External Userspace。`qmk_firmware` 本体には一切手を入れずに、自分のキーマップと
キーボード定義をこのリポジトリだけで管理する。

## 構成

```
users/gunubin/              3キーボード共通のコード (sequential_combo)
keyboards/                  upstream に存在するキーボードのキーマップ
  crkbd/keymaps/gunubin/
  peej/lumberjack/keymaps/gunubin/
keyboards_ext/gunubin/      upstream に存在しないキーボード定義 (※要シンボリックリンク)
  cornelius/                Cornelius rev2 (RP2040)
qmk.json                    ビルドターゲット定義
```

### keyboards_ext について

QMK の userspace が探索するのは `keyboards/*/keymaps/*`、`users/*`、`layouts/*` だけで、
**キーボード定義そのものは userspace に置けない**（`builddefs/build_keyboard.mk` 参照）。

Cornelius rev2 (RP2040) は upstream QMK に存在しないため、定義をここに置いた上で
`qmk_firmware/keyboards/gunubin` からシンボリックリンクで見せている。
upstream のファイルは1バイトも変更しないので `git pull` が衝突しない。

## セットアップ

```bash
git clone <this repo> ~/works/gunubin/qmk_userspace
git clone --filter=blob:none https://github.com/qmk/qmk_firmware.git ~/works/gunubin/qmk_firmware
cd ~/works/gunubin/qmk_firmware && make git-submodule

# 外部キーボード定義をリンク（これを忘れると gunubin/cornelius がビルドできない）
ln -sfn ~/works/gunubin/qmk_userspace/keyboards_ext/gunubin ~/works/gunubin/qmk_firmware/keyboards/gunubin

qmk config user.qmk_home=~/works/gunubin/qmk_firmware
qmk config user.overlay_dir=~/works/gunubin/qmk_userspace
qmk userspace-doctor
```

## ビルド

```bash
qmk userspace-compile              # qmk.json の全ターゲット
qmk compile -kb gunubin/cornelius -km gunubin   # 個別
```

成果物はこのディレクトリ直下に `*.uf2` / `*.hex` として出力される。

## ビルドターゲットの追加

```bash
qmk userspace-add -kb crkbd/rev1 -km gunubin
```

crkbd はキーマップだけ置いてあり、ビルドターゲット未登録。使うリビジョンが決まったら上記で追加する。

## upstream 追従

```bash
cd ~/works/gunubin/qmk_firmware && git pull
```

自分の変更が upstream 側に一切無いため、衝突は起きない。
