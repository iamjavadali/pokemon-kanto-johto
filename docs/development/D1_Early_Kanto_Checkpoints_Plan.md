# D1 — Early Kanto Story Checkpoints

## Purpose

Extend the accepted D0/D0 Fix1 Golden Yellow checkpoint architecture from the opening/Rival spine into the first major Yellow Kanto progression block. Checkpoints reconstruct cumulative story state; they are developer validation aids and do not replace actual Yellow scene implementation.

## Starting baseline

- Canonical branch: `prototype/v0.1`
- Starting HEAD: `07e5e4fb5b9e26c49eb896f0ac417d6fcf046633`
- D0 Fix1: manually gameplay-validated and accepted.

## Checkpoints

1. Before Brock
2. Mt. Moon Fossil
3. Mt. Moon Jessie & James
4. Before Misty
5. Nugget Bridge Rocket
6. Before Bill
7. Bulbasaur Gift
8. Charmander Gift
9. After S.S. Anne Rival / Before Captain
10. Before Lt. Surge
11. Squirtle Gift

## Architecture

- Preserve the accepted opening reconstruction helpers and all existing Rival checkpoints.
- Append new checkpoint enum IDs after existing IDs so D0/Rival identities remain stable.
- Build cumulative helpers for early Kanto state boundaries.
- Keep story-state reconstruction separate from test-party construction.
- Use explicit badge masks for new progression state rather than relying conceptually on badge count.
- Use existing FRLG flags/vars where they already represent the same persistent state (fossil, Bill, S.S. Ticket, HM01, Route 24 Rocket, ship state).
- Yellow-only scenes that have not yet been implemented (Jessie/James and Yellow starter gifts) receive scaffold checkpoints that reconstruct the state immediately before their future scene; their eventual scene-owned flags must be added when those scenes are implemented.

## Validation

- Strict changed-source preflight.
- Full strict FireRed build.
- ROM/header/provenance validation.
- Artifact upload.
- Manual gameplay validation of checkpoint destinations and inherited state is required before D1 is Accepted.
