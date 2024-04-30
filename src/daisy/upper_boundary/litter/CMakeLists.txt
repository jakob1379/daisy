add_subdirectory(components)
target_sources(${DAISY_CORE_NAME} PRIVATE
  litter_model.C
  litter_none_model.C
  litter_permanent_model.C
  litter_residue_model.C
  litter_mulch_model.C
  retention_exponential_model.C
  retention_pastis_model.C
)
