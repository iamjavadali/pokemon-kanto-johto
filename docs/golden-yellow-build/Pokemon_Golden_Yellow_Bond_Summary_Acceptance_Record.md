# Pokémon Golden Yellow — Bond Summary Acceptance Record

**Document type:** Dedicated implementation and manual-acceptance record  
**Project:** Pokémon Golden Yellow  
**Repository:** `iamjavadali/pokemon-kanto-johto`  
**Canonical working branch:** `prototype/v0.1`  
**Acceptance date:** September 6, 2026  
**Status:** ACCEPTED

---

# 1. Purpose

This record closes the accepted Partner-aware Bond Summary page and preserves the tested UI, data, and navigation behavior as a regression baseline.

It supplements the cumulative `Pokemon_Golden_Yellow_Acceptance_Log.md`. The live repository remains authoritative for implementation details.

# 2. Accepted Player-Facing Scope

The Pokémon Summary Screen includes a fifth `BOND` tab.

For ordinary Pokémon, the page exposes:

- friendship value and tier;
- whether the Pokémon is the active follower;
- follower style;
- steps remaining until the next friendship check.

For canonical Partner Pikachu, the page additionally exposes:

- current mood value;
- current feeling/reaction label;
- steps remaining until the next friendship check;
- exact steps until mood returns to neutral.

The friendship counter describes the next 128-step friendship check, not a guaranteed friendship gain. Boxed Pokémon identify their party-only step behavior.

# 3. Accepted Layout and Navigation

The Bond page deliberately reuses the Moves-page interaction model.

Accepted behavior:

1. the normal page remains clean and non-crowded;
2. pressing `A` enters the Bond stat list;
3. `Up/Down` moves through the rows and scrolls the four-row viewport;
4. pressing `A` on a row opens that stat's description;
5. `Up/Down` pages a long description;
6. pressing `B` returns one level.

The standard left green portrait panel is unchanged from the other summary tabs. No Bond-only effects box is drawn there.

The `BOND STATS` title uses the Moves-style small centered presentation, remains fully visible, and does not overlap the stat table. The row selector aligns with each visible row after the title/table offset. Extra rows, including step counters, remain clipped to the table until scrolling reveals them.

# 4. Partner and Follower Identity Rules

The canonical Partner remains `SPECIES_PIKACHU_STARTER`.

Partner following status is resolved from the live Partner Pokémon/follower object state. It must not use the separate follower-NPC subsystem, which previously caused an active Partner to display `Not following`.

Ordinary Pokémon continue to use the existing compatible follower infrastructure. The Bond page is informational and does not replace or redefine either follower system.

# 5. Implementation Lineage

- `d40bf97493e90e6d4e70749a91c275802e7f626e` — `feat: add bond summary page`
- `cbb0c743ea40b9bbd433551ad639f7343552ecf0` — `fix: redesign Bond summary interaction`
- `abd064954dd9db0f8bae3f7ddf6c1f12e01a848b` — `fix: refine Bond stats navigation and step tracking`
- `cc02d417c33a339775fad785d876629a05b64855` — `fix: align Bond page with Moves layout`
- `b6055ad8d5042d696154885a32fd71bc9c37b26d` — `fix: prevent Bond Stats title clipping`
- `398e2254b3790c8e65e1fa8b7bce0c1d9dd9d868` — `fix: align Bond row selector`

# 6. Final Build Evidence

**Source commit:** `398e2254b3790c8e65e1fa8b7bce0c1d9dd9d868`  
**Workflow:** `Prototype FireRed Development`  
**Workflow run:** `34017871548`  
**Result:** PASS  
**Artifact ID:** `9984550366`  
**Artifact name:** `pokemon-kanto-johto-firered-398e2254b3790c8e65e1fa8b7bce0c1d9dd9d868`  
**Artifact archive digest:** `sha256:4eea2e6a9a4c845adce4b4c0717dbc39ea67f01d34e61694b1401ade996f8084`

# 7. Manual Acceptance

The user tested the iterative Bond layouts, identified the remaining title and selector alignment defects, and explicitly marked the final corrected version as a pass.

The acceptance covers the final layout, visible data, nested navigation, scrolling behavior, follower-status correction, title visibility, and selector alignment described in this record.

# 8. Protected Regression Invariants

Future summary-screen or Partner changes must preserve:

- the existing Info, Skills, Battle Moves, and Contest Moves behavior;
- the unchanged standard left portrait panel;
- the four-row contained Bond viewport;
- nested `A`/arrow/`B` navigation;
- readable paged descriptions;
- canonical Partner identity;
- accurate live following status;
- correct friendship-check and mood-neutral step semantics;
- unclipped header and correctly aligned row selector;
- the accepted P1–P9 Partner reaction/audio baseline.
