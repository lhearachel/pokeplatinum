# This build file defines sets of constants which are used throughout the code
# base, both in the ROM's source code and in internal tooling. We generate these
# constants to ensure that they have a particular set of characteristics, are
# easily validated in data files representing them as strings, and can be easily
# parsed by simple string manipulations.

# This must be declared here instead of in tools/meson.build, as some tools depend
# on metang-generated constants.
subproject('metang', default_options: ['c_std=c99'])

metang_exe = find_program('metang', native: true)

metang_generators = {
    'abilities': { 'tag': 'Ability' },
    'accessories': {'tag': 'Accessory'},
    'ai_action_choices': { 'tag': 'AIActionChoice' },
    'ai_flags': { 'tag': 'AIFlag', 'extra': ['--bitmask'] },
    'ai_load_type_targets': { 'tag': 'AILoadTypeTarget' },
    'ai_weather_types': { 'tag': 'AIWeatherType' },
    'backdrops': { 'tag': 'Backdrop' },
    'badges': { 'tag': 'Badge' },
    'battle_actions': { 'tag': 'BattleAction' },
    'battle_backgrounds': { 'tag': 'BattleBackground' },
    'battle_boot_states': { 'tag': 'BattleBootState' },
    'battle_context_params': { 'tag': 'BattleContextParam' },
    'battle_message_tags': { 'tag': 'BattleMessageTag' },
    'battle_mon_params': { 'tag': 'BattleMonParam' },
    'battle_move_effects': { 'tag': 'BattleMoveEffect' },
    'battle_move_subscript_ptrs': { 'tag': 'BattleMoveSubscriptPtr' },
    'battle_script_battlers': { 'tag': 'Battler' },
    'battle_script_check_side_condition_ops': { 'tag': 'BattleScriptCheckSideConditionOp' },
    'battle_script_opcodes': { 'tag': 'BattleScriptOpCode' },
    'battle_script_side_conditions': { 'tag': 'BattleScriptSideCondition' },
    'battle_script_turn_flags': { 'tag': 'BattleScriptTurnFlag' },
    'battle_script_vars': { 'tag': 'BattleScriptVars' },
    'battle_side_effect_types': { 'tag': 'BattleSideEffectType' },
    'battle_stats': { 'tag': 'BattleStat' },
    'battle_sub_animations': { 'tag': 'BattleSubAnimation' },
    'battle_subscripts': { 'tag': 'BattleSubscript' }, # functionally pre-generating the NARC index
    'battle_terrains': { 'tag': 'BattleTerrain' },
    'bg_event_dirs': { 'tag': 'BgEventDir' },
    'catching_show_points_category': { 'tag': 'CatchingShowPointsCategory' },
    'days_of_week': { 'tag': 'DayOfWeek' },
    'distribution_events': { 'tag': 'DistributionEvent' },
    'egg_groups': { 'tag': 'EggGroup' },
    'evolution_methods': { 'tag': 'EvolutionMethod' },
    'exp_rates': { 'tag': 'ExpRate' },
    'game_records': { 'tag': 'GameRecord' },
    'first_arrival_to_zones': { 'tag': 'FirstArrivalToZone' },
    'footprint_sizes': { 'tag': 'FootprintSize' },
    'gender_ratios': { 'tag': 'GenderRatio' },
    'genders': { 'tag': 'Gender' },
    'giratina_shadow_animations': {'tag': 'GiratinaShadowAnimation'},
    'hidden_locations': { 'tag': 'HiddenLocation' },
    'item_ai_categories': { 'tag': 'ItemAICategory' },
    'item_battle_categories': { 'tag': 'ItemBattleCategory' },
    'item_hold_effects': { 'tag': 'ItemHoldEffect' },
    'items': { 'tag': 'Item' },
    'journal_location_events': { 'tag': 'JournalLocationEventType' },
    'journal_online_events': { 'tag': 'JournalOnlineEventType' },
    'map_headers': { 'tag': 'MapHeader' },
    'maps': { 'tag': 'MapID' },
    'move_attributes': { 'tag': 'MoveAttribute' },
    'move_classes': { 'tag': 'MoveClass' },
    'move_flags': { 'tag': 'MoveFlag', 'extra': ['--bitmask'] },
    'move_ranges': { 'tag': 'MoveRange', 'extra': ['--bitmask'] },
    'movement_actions': { 'tag': 'MovementAction' },
    'movement_types': { 'tag': 'MovementType' },
    'moves': { 'tag': 'Move' },
    'natures': { 'tag': 'Nature' },
    'npc_trades': { 'tag': 'NpcTradeID' },
    'object_events': { 'tag': 'ObjectEventGfx' },
    'pal_park_land_area': { 'tag': 'PalParkLandArea' },
    'pal_park_water_area': { 'tag': 'PalParkWaterArea' },
    'pokemon_body_shapes': { 'tag': 'PokemonBodyShape' },
    'pokemon_colors': { 'tag': 'PokemonColor' },
    'pokemon_contest_types': { 'tag': 'PokemonContestType' },
    'pokemon_data_params': { 'tag': 'PokemonDataParam' },
    'pokemon_stats': { 'tag': 'PokemonStat' },
    'pokemon_types': { 'tag': 'PokemonType' },
    'poketch_apps': { 'tag': 'PoketchAppID' },
    'ribbons': { 'tag': 'RibbonID' },
    'roaming_slots': { 'tag': 'RoamingSlot' },
    'save_types': { 'tag': 'SaveType' },
    'sdat': { 'tag': 'SDATID' },
    'signpost_commands': { 'tag': 'SignpostCommand' },
    'signpost_types': { 'tag': 'SignpostType' },
    'size_contest_results': { 'tag': 'SizeContestResult' },
    'shadow_sizes': { 'tag': 'ShadowSize' },
    'species': { 'tag': 'Species' },
    'species_data_params': { 'tag': 'SpeciesDataParam' },
    'string_padding_mode': { 'tag': 'PaddingMode' },
    'text_banks': { 'tag': 'TextBank' },
    'trainers': { 'tag': 'TrainerID' },
    'trainer_classes': { 'tag': 'TrainerClass' },
    'trainer_score_events': { 'tag': 'TrainerScoreEvent' },
    'trainer_types': { 'tag': 'TrainerType' },
    'tutor_locations': {'tag': 'TutorLocation'},
    'vars_flags': {'tag': 'VarFlag'},
    'versions': { 'tag': 'Version' },
    'villa_furnitures': {'tag': 'VillaFurniture'},
    'mart_specialties_id': {'tag': 'MartSpecialtiesID'},
    'mart_decor_id': {'tag': 'MartDecorID'},
    'mart_seal_id': {'tag': 'MartSealID'},
    'mart_frontier_id': {'tag': 'MartFrontierId'},
    'mystery_gift_delivery_stages': {'tag': 'MysteryGiftDeliveryStage'},
}

c_consts_generators = []
py_consts_generators = []

foreach gen_key : metang_generators.keys()
    gen_h = gen_key + '.h'
    gen_py = gen_key + '.py'
    gen_file = files(gen_key + '.txt')
    gen_args = metang_generators.get(gen_key)
    gen_extra_args = gen_args.get('extra', [])

    c_consts_generators += custom_target(gen_h,
        output: gen_h,
        input: gen_file,
        command: [
            metang_exe,
            '--tag', gen_args['tag'],
            '--guard', 'POKEPLATINUM_GENERATED',
            '--output', '@OUTPUT@',
            gen_extra_args,
            '@INPUT@',
        ]
    )

    py_consts_generators += custom_target(gen_py,
        output: gen_py,
        input: gen_file,
        command: [
            metang_exe,
            '--tag', gen_args['tag'],
            '--output', '@OUTPUT@',
            gen_extra_args,
            '@INPUT@',
        ]
    )
endforeach

npc_trades_txt = files('npc_trades.txt')
species_txt = files('species.txt')
text_banks_txt = files('text_banks.txt')
trainers_txt = files('trainers.txt')

py_consts_env = environment()
py_consts_env.append('PYTHONPATH', meson.project_build_root())
