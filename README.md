# Vintage Radio – Bedrock Addon

A detailed placeable **vintage radio** for Minecraft Bedrock **1.26.33+**.

## Features

- Craftable / giveable **Vintage Radio** item
- Place on any block face (sits on top when you click the top face)
- **Interact** (right-click / long-press) or punch to **toggle ON / OFF**
- Sneak + attack to damage/break (drops the radio)
- **Turn-on / turn-off bone animations** (knobs spin, antenna wiggles, body bounce, LED pulse, display lights)
- **While playing**: speaker thumps, body grooves, knobs drift, antenna sways
- **Particles**: music notes, radio waves, power-on sparkles, power-off embers, speaker pulses
- **Song**: *Lose My Mind – F1 The Extended Cinematic Cut* (~5:22, loops)
- Glowing ON texture + light emission on the block variant

## Install

1. Download `Vintage_Radio.mcaddon`
2. Open it with Minecraft Bedrock (double-click / share sheet → Minecraft)
3. Create or edit a world → enable **Vintage Radio** behavior + resource packs
4. Enable **Beta APIs** / **Holiday Creator Features** if prompted (Script API)

## Crafting

```
 I N I
 N R N
 I I I
```

- `I` = Iron Ingot  
- `N` = Note Block  
- `R` = Redstone Dust  

Or: `/give @s spectra:radio`

## Controls

| Action | Result |
|--------|--------|
| Use radio item on a block | Place radio |
| Interact / right-click radio | Toggle power |
| Punch radio (not sneaking) | Toggle power |
| Sneak + attack | Damage / break |
| Break | Drops radio item |

## Contents

| Pack | Contents |
|------|----------|
| Behavior | Entity, item, recipe, block, script |
| Resource | Geometry, textures, animations, particles, sounds |

## Version

- `min_engine_version`: 1.26.0  
- Script module: `@minecraft/server` 2.0.0  
- Tested target: Bedrock **1.26.33**
