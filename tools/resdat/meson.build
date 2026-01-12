project('rust testing', 'rust')

manifest = files('Cargo.toml')
target_dir = meson.current_build_dir()
cargo_wrap = find_program('buildit.sh', native: true)

custom_target(
    'resdat',
    output: 'resdat',
    input: files('src/main.rs'),
    command: [
        cargo_wrap,
        '--manifest-path', manifest,
        '--target-dir', target_dir,
        '@OUTPUT@',
    ],
    build_by_default: true,
)
