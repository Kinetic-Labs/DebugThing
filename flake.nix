{
  description = "DebugThing development env";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pico-sdk = pkgs.fetchFromGitHub {
          owner = "raspberrypi";
          repo = "pico-sdk";
          rev = "2.1.0";
          hash = "sha256-nLn6H/P79Jbk3/TIowH2WqmHFCXKEy7lgs7ZqhqJwDM=";
          fetchSubmodules = true;
        };
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            cmake
            gcc-arm-embedded
            picotool
            python3
            git
            minicom
            screen
          ];
          shellHook = ''
            export PICO_SDK_PATH=${pico-sdk}
          '';
        };
        packages.default = pkgs.stdenv.mkDerivation {
          name = "debug_thing";
          src = ./.;
          nativeBuildInputs = with pkgs; [
            cmake
            gcc-arm-embedded
            python3
            git
            picotool
          ];
          PICO_SDK_PATH = pico-sdk;
          PICO_NO_PICOTOOL = "1";
          configurePhase = ''
            mkdir -p build
            cd build
            cmake .. -DPICO_NO_PICOTOOL=1
          '';
          buildPhase = ''
            make
            if [ ! -f debug_thing.uf2 ] && [ -f debug_thing.elf ]; then
              ${pkgs.picotool}/bin/picotool uf2 convert debug_thing.elf debug_thing.uf2
            fi
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp debug_thing.uf2 $out/bin/ || echo "warning: no UF2 file found"
            cp debug_thing.elf $out/bin/ || echo "warning: no ELF file found"
            cp debug_thing.bin $out/bin/ || echo "warning: no BIN file found"
          '';
        };
        apps = {
          default = {
            type = "app";
            program = toString (
              pkgs.writeShellScript "flash-pico" ''
                UF2_FILE=$(ls ${self.packages.${system}.default}/bin/*.uf2 | head -n1)
                if [ -z "$UF2_FILE" ]; then
                  echo "Error: No UF2 file found"
                  exit 1
                fi
                echo "flashing $UF2_FILE"
                ${pkgs.picotool}/bin/picotool load -u -v -x "$UF2_FILE"
              ''
            );
          };
          flash = {
            type = "app";
            program = toString (
              pkgs.writeShellScript "flash-pico" ''
                UF2_FILE=$(ls ${self.packages.${system}.default}/bin/*.uf2 | head -n1)
                if [ -z "$UF2_FILE" ]; then
                  echo "error: No UF2 file found"
                  exit 1
                fi
                echo "flashing $UF2_FILE"
                ${pkgs.picotool}/bin/picotool load -u -v -x "$UF2_FILE"
              ''
            );
          };
          serial = {
            type = "app";
            program = toString (
              pkgs.writeShellScript "watch-serial" ''
                DEVICE=""
                
                for dev in /dev/cu.usbmodem* /dev/cu.usbserial*; do
                  if [ -e "$dev" ]; then
                    DEVICE="$dev"
                    break
                  fi
                done
                
                if [ -z "$DEVICE" ]; then
                  for dev in /dev/ttyACM* /dev/ttyUSB*; do
                    if [ -e "$dev" ]; then
                      DEVICE="$dev"
                      break
                    fi
                  done
                fi

                if [ -z "$DEVICE" ]; then
                  echo "error: no serial device found"
                  echo "looking for:"
                  echo "  macOS: /dev/cu.usbmodem* or /dev/cu.usbserial*"
                  echo "  Linux: /dev/ttyACM* or /dev/ttyUSB*"
                  echo ""
                  echo "make sure your Pico is connected and the program is running"
                  exit 1
                fi
                
                ${pkgs.screen}/bin/screen "$DEVICE" 115200
              ''
            );
          };
        };
      }
    );
}

