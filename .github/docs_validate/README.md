# Documentation summary validation

The prototype FireRed workflow runs `.github/docs_validate/inclusive_summary.py` as part of its source sanity gate.

Every Markdown file under `docs/` must be referenced from `docs/SUMMARY.md`, except exclusions explicitly handled by the validator. When adding or moving project documentation, update `docs/SUMMARY.md` in the same change set so source-validation builds do not fail on an unrelated documentation-index error.
