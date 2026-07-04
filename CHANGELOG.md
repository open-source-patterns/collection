# Changelog

All notable changes to this project will be documented in this file.

## [1.2.0] - 2026-07-03

### Added
- Cross-platform `Mutex` and `MutexOnce` abstractions.
- Process resource usage utilities.
- Unit tests for `Array`, `Dictionary`, and `Mutex`.

### Changed
- Renamed the public API to use consistent `snake_case` naming.
- Improved CMake configuration with optional sanitizer support.
- Improved cross-platform compatibility of the test suite.

### Fixed
- Cross-platform portability issues in tests and examples.
- Minor implementation and build improvements.

## [1.1.0] - 2026-05-17
### Added
- Opaque caller-managed memory initialization pattern for collections
- Optional destructor callbacks for collection deinitialization

## [1.0.1] - 2026-01-26
### Added
- In-out error parameters for memory allocation functions
- Unit tests for `Mutex`
- `DEVELOPMENT.md`

### Removed
- `index` parameter from `forEach`

## [1.0.0] - 2025-06-20
### Added
- Initial release of the library
- Basic data structures: Array and Dictionary implemented
- Public API for adding, removing, and accessing items
- CMake support for building, installing, and exporting targets
- Documentation and examples included