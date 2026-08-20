/**
 * Vintage Radio – Bedrock 1.26+
 * Place the radio item, then interact (right-click / tap) to toggle power.
 * Plays "Lose My Mind – F1 Extended Cinematic Cut" while powered on.
 */
import {
  world,
  system,
  ItemStack,
  EquipmentSlot
} from "@minecraft/server";

const RADIO_ENTITY = "spectra:radio";
const RADIO_ITEM = "spectra:radio";
const MUSIC_SOUND = "spectra.radio.music";
const CLICK_ON = "spectra.radio.on";
const CLICK_OFF = "spectra.radio.off";
/** Song length in ticks (5:22 @ 20 tps) */
const SONG_LENGTH_TICKS = 6440;
const MUSIC_RANGE = 48;
const LOOP_CHECK = 20;

/** @type {Map<string, { entityId: string, dimensionId: string, playing: boolean, startedAt: number, soundId?: number }>} */
const radios = new Map();

function keyOf(entity) {
  return entity.id;
}

function getPowered(entity) {
  try {
    return entity.getProperty("spectra:powered") === true;
  } catch {
    return false;
  }
}

function setPowered(entity, on) {
  try {
    entity.setProperty("spectra:powered", on);
  } catch (e) {
    // Fallback via entity events if property write fails
    try {
      entity.triggerEvent(on ? "spectra:force_on" : "spectra:force_off");
    } catch (_) {}
  }
}

function playMusic(entity) {
  const dim = entity.dimension;
  const loc = entity.location;
  try {
    // Stop any previous instance at this spot first
    dim.runCommand(
      `stopsound @a[r=${MUSIC_RANGE}] ${MUSIC_SOUND}`
    );
  } catch (_) {}

  try {
    // Play streamed music for nearby players
    dim.runCommand(
      `playsound ${MUSIC_SOUND} @a[r=${MUSIC_RANGE}] ${loc.x.toFixed(2)} ${loc.y.toFixed(2)} ${loc.z.toFixed(2)} 3.0 1.0 1.0`
    );
  } catch (e) {
    try {
      entity.dimension.playSound(MUSIC_SOUND, loc, {
        volume: 3.0,
        pitch: 1.0
      });
    } catch (_) {}
  }

  // Ambient particle burst when music starts
  try {
    dim.spawnParticle("spectra:radio_power_on", {
      x: loc.x,
      y: loc.y + 0.4,
      z: loc.z
    });
  } catch (_) {}
}

function stopMusic(entity) {
  const dim = entity.dimension;
  const loc = entity.location;
  try {
    dim.runCommand(
      `stopsound @a[r=${MUSIC_RANGE + 8}] ${MUSIC_SOUND}`
    );
  } catch (_) {}
  try {
    dim.spawnParticle("spectra:radio_power_off", {
      x: loc.x,
      y: loc.y + 0.4,
      z: loc.z
    });
  } catch (_) {}
}

function spawnAmbientParticles(entity) {
  const dim = entity.dimension;
  const loc = entity.location;
  const base = { x: loc.x, y: loc.y + 0.35, z: loc.z };
  try {
    dim.spawnParticle("spectra:radio_notes", {
      x: base.x + (Math.random() - 0.5) * 0.3,
      y: base.y + Math.random() * 0.2,
      z: base.z - 0.2
    });
  } catch (_) {}
  if (Math.random() > 0.5) {
    try {
      dim.spawnParticle("spectra:radio_waves", {
        x: base.x,
        y: loc.y + 0.95,
        z: base.z
      });
    } catch (_) {}
  }
  if (Math.random() > 0.7) {
    try {
      dim.spawnParticle("spectra:radio_speaker", {
        x: base.x - 0.1,
        y: base.y,
        z: base.z - 0.25
      });
    } catch (_) {}
  }
}

function registerRadio(entity) {
  const id = keyOf(entity);
  if (!radios.has(id)) {
    radios.set(id, {
      entityId: id,
      dimensionId: entity.dimension.id,
      playing: false,
      startedAt: 0
    });
  }
  return radios.get(id);
}

function toggleRadio(entity, player) {
  const state = registerRadio(entity);
  const currentlyOn = getPowered(entity);
  const next = !currentlyOn;

  setPowered(entity, next);

  // Also fire entity events so component groups / animation controllers sync
  try {
    entity.triggerEvent(next ? "spectra:force_on" : "spectra:force_off");
  } catch (_) {}

  const loc = entity.location;
  if (next) {
    try {
      entity.dimension.playSound(CLICK_ON, loc, { volume: 1.0, pitch: 1.1 });
    } catch (_) {
      try {
        entity.dimension.runCommand(
          `playsound ${CLICK_ON} @a[r=16] ${loc.x} ${loc.y} ${loc.z} 1 1.1`
        );
      } catch (__) {}
    }
    // Small delay so turn-on animation kicks in before music
    system.runTimeout(() => {
      if (!entity.isValid) return;
      if (!getPowered(entity)) return;
      playMusic(entity);
      state.playing = true;
      state.startedAt = system.currentTick;
    }, 8);
    try {
      player?.onScreenDisplay?.setActionBar("§a♪ Radio ON §7– Lose My Mind (F1)");
    } catch (_) {}
  } else {
    try {
      entity.dimension.playSound(CLICK_OFF, loc, { volume: 0.9, pitch: 0.9 });
    } catch (_) {
      try {
        entity.dimension.runCommand(
          `playsound ${CLICK_OFF} @a[r=16] ${loc.x} ${loc.y} ${loc.z} 0.9 0.9`
        );
      } catch (__) {}
    }
    stopMusic(entity);
    state.playing = false;
    state.startedAt = 0;
    try {
      player?.onScreenDisplay?.setActionBar("§c■ Radio OFF");
    } catch (_) {}
  }
}

// Track newly placed radios (entity_placer on the item handles spawning)
world.afterEvents.entitySpawn.subscribe((ev) => {
  const entity = ev.entity;
  if (!entity || entity.typeId !== RADIO_ENTITY) return;
  system.run(() => {
    if (!entity.isValid) return;
    setPowered(entity, false);
    registerRadio(entity);
    try {
      entity.dimension.playSound("dig.wood", entity.location, {
        volume: 1.0,
        pitch: 1.0
      });
    } catch (_) {}
  });
});

// Fallback placer if entity_placer is unavailable on older builds
world.beforeEvents.itemUseOn.subscribe((ev) => {
  const item = ev.itemStack;
  if (!item || item.typeId !== RADIO_ITEM) return;

  // Let native entity_placer run when present; still provide a script fallback
  // by spawning only if nothing appears (handled via short delayed check).
  const player = ev.source;
  const block = ev.block;
  const face = ev.blockFace;

  system.runTimeout(() => {
    try {
      const faceOffset = {
        Up: { x: 0.5, y: 1, z: 0.5 },
        Down: { x: 0.5, y: -0.5, z: 0.5 },
        North: { x: 0.5, y: 0, z: -0.5 },
        South: { x: 0.5, y: 0, z: 1.5 },
        East: { x: 1.5, y: 0, z: 0.5 },
        West: { x: -0.5, y: 0, z: 0.5 }
      };
      const off = faceOffset[face] || { x: 0.5, y: 1, z: 0.5 };
      const pos = {
        x: block.location.x + off.x,
        y: block.location.y + (face === "Up" ? 1 : face === "Down" ? -1 : 0),
        z: block.location.z + off.z
      };

      const nearby = player.dimension.getEntities({
        type: RADIO_ENTITY,
        location: pos,
        maxDistance: 1.5
      });
      if (nearby.length > 0) return; // entity_placer already worked

      const entity = player.dimension.spawnEntity(RADIO_ENTITY, {
        x: block.location.x + 0.5,
        y: block.location.y + 1,
        z: block.location.z + 0.5
      });
      try {
        const dx = player.location.x - entity.location.x;
        const dz = player.location.z - entity.location.z;
        entity.setRotation({ x: 0, y: (Math.atan2(-dx, dz) * 180) / Math.PI });
      } catch (_) {}
      setPowered(entity, false);
      registerRadio(entity);

      try {
        const gm = String(player.getGameMode?.() ?? "").toLowerCase();
        if (!gm.includes("creative")) {
          const equip = player.getComponent("minecraft:equippable");
          const main = equip?.getEquipment(EquipmentSlot.Mainhand);
          if (main && main.typeId === RADIO_ITEM) {
            if (main.amount > 1) {
              main.amount -= 1;
              equip.setEquipment(EquipmentSlot.Mainhand, main);
            } else {
              equip.setEquipment(EquipmentSlot.Mainhand, undefined);
            }
          }
        }
      } catch (_) {}

      player.onScreenDisplay?.setActionBar(
        "§eVintage Radio placed §7– interact to power on"
      );
    } catch (e) {
      console.warn("[Radio] fallback place: " + e);
    }
  }, 2);
});

// --- Interact with placed radio (player hit / interact) ---
world.beforeEvents.playerInteractWithEntity.subscribe((ev) => {
  const entity = ev.target;
  if (!entity || entity.typeId !== RADIO_ENTITY) return;
  ev.cancel = true;
  const player = ev.player;
  system.run(() => {
    if (!entity.isValid) return;
    toggleRadio(entity, player);
  });
});

// Also support attacking the radio gently as toggle (sneak+attack = break)
world.afterEvents.entityHitEntity.subscribe((ev) => {
  const entity = ev.hitEntity;
  const attacker = ev.damagingEntity;
  if (!entity || entity.typeId !== RADIO_ENTITY) return;
  if (!attacker || attacker.typeId !== "minecraft:player") return;

  const player = attacker;
  // If sneaking, allow damage/break; otherwise toggle
  if (player.isSneaking) {
    return;
  }

  system.run(() => {
    if (!entity.isValid) return;
    // Heal back the punch damage so it acts as a button
    try {
      const health = entity.getComponent("minecraft:health");
      if (health) health.resetToMaxValue();
    } catch (_) {}
    toggleRadio(entity, player);
  });
});

// Cleanup when radio dies / removed
world.afterEvents.entityRemove.subscribe((ev) => {
  const id = ev.removedEntityId;
  const state = radios.get(id);
  if (!state) return;
  radios.delete(id);
  try {
    const dim = world.getDimension(ev.typeId ? state.dimensionId : state.dimensionId);
    dim.runCommand(`stopsound @a[r=64] ${MUSIC_SOUND}`);
  } catch (_) {
    try {
      world.getDimension("overworld").runCommand(`stopsound @a ${MUSIC_SOUND}`);
    } catch (__) {}
  }
});

world.afterEvents.entityDie.subscribe((ev) => {
  const entity = ev.deadEntity;
  if (!entity || entity.typeId !== RADIO_ENTITY) return;
  stopMusic(entity);
  radios.delete(keyOf(entity));

  // Drop the radio item
  try {
    const dim = entity.dimension;
    const loc = entity.location;
    dim.spawnItem(new ItemStack(RADIO_ITEM, 1), loc);
  } catch (_) {}
});

// Loop: particles + music restart when song ends
system.runInterval(() => {
  const toDelete = [];
  for (const [id, state] of radios) {
    let entity;
    try {
      entity = world.getEntity(id);
    } catch {
      entity = undefined;
    }
    if (!entity || !entity.isValid) {
      toDelete.push(id);
      continue;
    }

    const powered = getPowered(entity);
    if (powered) {
      spawnAmbientParticles(entity);
      // Restart music after song length
      if (state.playing && system.currentTick - state.startedAt >= SONG_LENGTH_TICKS) {
        playMusic(entity);
        state.startedAt = system.currentTick;
      }
      if (!state.playing) {
        playMusic(entity);
        state.playing = true;
        state.startedAt = system.currentTick;
      }
    } else if (state.playing) {
      stopMusic(entity);
      state.playing = false;
    }
  }
  for (const id of toDelete) radios.delete(id);
}, LOOP_CHECK);

// Give radio on first join (helpful for testing) + tell how to use
world.afterEvents.playerSpawn.subscribe((ev) => {
  if (!ev.initialSpawn) return;
  const player = ev.player;
  system.runTimeout(() => {
    try {
      player.sendMessage(
        "§6§lVintage Radio§r §7– craft with §fIron + Note Blocks + Redstone§7, or §e/give @s spectra:radio"
      );
      player.sendMessage(
        "§7Place it, then §einteract / right-click§7 to turn §aON§7/§cOFF§7. Song: §dLose My Mind (F1)"
      );
    } catch (_) {}
  }, 40);
});

// Custom component registration (1.21.20+ style) – safe no-op if unavailable
system.beforeEvents?.startup?.subscribe?.((init) => {
  try {
    init.itemComponentRegistry?.registerCustomComponent?.("spectra:radio_item", {
      onUseOn: () => {
        // Handled globally above for broader version support
      }
    });
  } catch (_) {}
  try {
    init.blockComponentRegistry?.registerCustomComponent?.("spectra:radio_behavior", {
      onPlayerInteract: (e) => {
        // Block variant support if someone places the block form
        const block = e.block;
        const player = e.player;
        try {
          const powered = block.permutation.getState("spectra:powered") === 1;
          const next = powered ? 0 : 1;
          block.setPermutation(
            block.permutation.withState("spectra:powered", next)
          );
          const loc = block.center();
          if (next === 1) {
            block.dimension.playSound(CLICK_ON, loc, { volume: 1, pitch: 1.1 });
            block.dimension.runCommand(
              `playsound ${MUSIC_SOUND} @a[r=${MUSIC_RANGE}] ${loc.x} ${loc.y} ${loc.z} 3 1`
            );
            block.dimension.spawnParticle("spectra:radio_power_on", loc);
            player?.onScreenDisplay?.setActionBar("§a♪ Radio ON");
          } else {
            block.dimension.playSound(CLICK_OFF, loc, { volume: 0.9, pitch: 0.9 });
            block.dimension.runCommand(
              `stopsound @a[r=${MUSIC_RANGE + 8}] ${MUSIC_SOUND}`
            );
            block.dimension.spawnParticle("spectra:radio_power_off", loc);
            player?.onScreenDisplay?.setActionBar("§c■ Radio OFF");
          }
        } catch (err) {
          console.warn("[Radio] block toggle: " + err);
        }
      }
    });
  } catch (_) {}
});

console.warn("[Vintage Radio] Loaded – Lose My Mind (F1 Extended Cinematic Cut)");
