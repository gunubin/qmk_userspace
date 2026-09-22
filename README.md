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

## フラッシュ

`qmk flash` はビルドしてからブートローダー状態のデバイスを待ち受ける。
先にコマンドを実行してから、キーボードをブートローダーに入れる方がやりやすい。

```bash
qmk flash -kb gunubin/cornelius -km gunubin
qmk flash -kb peej/lumberjack  -km gunubin
```

### ブートローダーへの入り方

どのキーマップにも `QK_BOOT` を割り当てていないため、キー操作では入れない。物理的な操作が必要。

| キーボード | MCU / bootloader | 入り方 |
|---|---|---|
| `gunubin/cornelius` | RP2040 / rp2040 | 左上キー（matrix `[0,0]`）を押しながら USB を挿す。または基板の BOOTSEL ボタンを押しながら挿す |
| `peej/lumberjack` | atmega328p / usbasploader | 基板側のブートジャンパ／ボタンを操作しながら挿す（[hsgw/USBaspLoader](https://github.com/hsgw/USBaspLoader/tree/plaid) 仕様。具体的な位置は自分の基板で確認） |

cornelius は左上キー押しながら挿すと `RPI-RP2` という USB ドライブとしてマウントされる。
この状態なら `qmk flash` を使わず、ビルド済みの `.uf2` をドラッグするだけでもよい。

```bash
qmk userspace-compile
cp gunubin_cornelius_gunubin.uf2 /Volumes/RPI-RP2/
```

lumberjack は usbasploader なので avrdude 経由になる。`qmk flash` に任せるのが確実。

### 補足: リセットキーを付けておくと楽

ブートローダー用の物理操作が面倒なら、キーマップの空きキーに `QK_BOOT` を割り当てておくと
そのキーだけでブートローダーに入れる。現状はどのキーマップにも入っていない。

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
