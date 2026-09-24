# qmk_userspace (gunubin)

QMK External Userspace。`qmk_firmware` 本体には一切手を入れずに、自分のキーマップと
キーボード定義をこのリポジトリだけで管理する。

## 構成

```
users/gunubin/              全機種共通のコード (sequential_combo)
keyboards/                  upstream に存在するキーボードのキーマップ
  crkbd/keymaps/gunubin/
  peej/lumberjack/keymaps/gunubin/
keyboards_ext/gunubin/      upstream に存在しないキーボード定義 (※要シンボリックリンク)
  cornelius/                Cornelius rev2 (RP2040)
  duo/                      LAZYDESIGNERS duo (2024年版 / STM32F072)
qmk.json                    ビルドターゲット定義
```

### keyboards_ext について

QMK の userspace が探索するのは `keyboards/*/keymaps/*`、`users/*`、`layouts/*` だけで、
**キーボード定義そのものは userspace に置けない**（`builddefs/build_keyboard.mk` 参照）。

Cornelius rev2 (RP2040) と LAZYDESIGNERS duo は upstream QMK に存在しないため、定義をここに置いた上で
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

```bash
qmk flash -kb crkbd/rev4_1/standard -km gunubin
qmk flash -kb gunubin/cornelius     -km gunubin
qmk flash -kb gunubin/duo           -km gunubin
qmk flash -kb peej/lumberjack       -km gunubin
```

`qmk flash` はビルドしてからブートローダー状態のデバイスを待ち受ける。
先にコマンドを実行してから、キーボードをブートローダーに入れる方がやりやすい。

### ブートローダーへの入り方

`_NUMBER` レイヤーの **B の位置**に `QK_BOOT` を割り当ててある（全機種とも同じ物理位置）。

```
右親指の L2_DEL を押しながら、左手人差し指で B
```

```
_NUMBER レイヤー

  crkbd / cornelius / duo    lumberjack
   1 2 3 4 5 | 6 7 8 9 0      ! @ # $ % | ^ & * ( )
   . . . . . | . . . . .      1 2 3 4 5 | 6 7 8 9 0
   . . . .[B]| . . . . .      . . . . . | . . . . .
                              . . . .[B]| . . . . .

  [B] = QK_BOOT
```

レイヤー2は左親指の `LM_NUM` でも入れるが、そちらは `MOD_LGUI` が同時に乗る（`QK_BOOT` は
修飾キーの影響を受けないので動作はする）。`L2_DEL` を使えば両手操作になり誤爆しにくい。

### 物理的に入る方法（ファームが壊れて QK_BOOT が効かないとき）

| キーボード | MCU / bootloader | 入り方 |
|---|---|---|
| `crkbd/rev4_1/standard` | RP2040 / rp2040 | 基板の BOOTSEL ボタンを押しながら USB を挿す |
| `gunubin/cornelius` | RP2040 / rp2040 | 左上キー（matrix `[0,0]`）を押しながら挿す。または BOOTSEL ボタン |
| `gunubin/duo` | STM32F072 / stm32-dfu | 左上キー（matrix `[0,0]`）を押しながら挿す（Bootmagic Lite）。最終手段は基板の boot ボタン＋リセットパッド短絡 |
| `peej/lumberjack` | atmega328p / usbasploader | 基板側のブートジャンパ／ボタンを操作しながら挿す（[hsgw/USBaspLoader](https://github.com/hsgw/USBaspLoader/tree/plaid) 仕様。位置は自分の基板で確認） |

RP2040 の2機種はブートローダーに入ると `RPI-RP2` という USB ドライブとしてマウントされる。
この状態なら `qmk flash` を使わずビルド済みの `.uf2` をドラッグするだけでもよい。

```bash
qmk userspace-compile
cp gunubin_cornelius_gunubin.uf2 /Volumes/RPI-RP2/
```

crkbd は分割キーボードなので、**左右それぞれをブートローダーに入れて2回フラッシュする**。

lumberjack は usbasploader なので avrdude 経由になる。`qmk flash` に任せるのが確実。

### duo の初回フラッシュ

まだ自分のファームを入れていない状態では `QK_BOOT` も Bootmagic も自分の設定ではないので、
出荷時ファーム側の手段を使う。duo の出荷時ファーム（LAZYDESIGNERS 製 vial keymap）には
レイヤー1の **2段目左端（レイヤー0 では Tab）** に `QK_BOOT` がある。

**この手順で DFU に入れることを実機で確認済み（2026-09-24）。**

```
左の 2u キー（Space / LT(1, KC_SPC)）を押しながら、2段目の左端キー
```

```
duo レイヤー0 最下段（9キー）

 idx    0      1      2      3      4      5      6      7      8
 幅   1.25u    1u   1.25u   2u     1u     2u   1.25u    1u   1.25u
      LCTL   LGUI   RALT  [SPC]   SPC    SPC   LEFT   DOWN   RGHT
                           ↑これを押しながら Tab の位置
```

出荷時ファームを書き換えてしまった場合など、上記が使えないときは readme が指定する
物理手段になる（未検証）。

> **Reset Key:** To enter the bootloader, press the boot button and short the reset pads
> together on the PCB. — vial-qmk `keyboards/lazydesigners/duo/readme.md`

boot ボタンとリセットパッドの物理的な位置は基板を見て確認すること（ガスケットマウントなので
ケースを開ける必要がある）。STM32F072 の標準手順で、boot ボタンを押したままリセットを
かけると DFU モードに入る。

自分のファームを入れたあとは、上の表の通り **左上キーを押しながら挿す**（Bootmagic Lite）か、
`_NUMBER` レイヤーの B（`QK_BOOT`）が使える。

### STM32 機種の成果物

duo は STM32 なので出力は `.bin` で、`qmk flash` は dfu-util 経由になる。

## ビルドターゲットの追加

```bash
qmk userspace-add -kb crkbd/rev1 -km gunubin
```

crkbd は `rev4_1/standard`（Corne v4 / RP2040）。過去のフラッシュ履歴から確定した。

## upstream 追従

```bash
cd ~/works/gunubin/qmk_firmware && git pull
```

自分の変更が upstream 側に一切無いため、衝突は起きない。
