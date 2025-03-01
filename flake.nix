{
  description = "GenerateEncoder";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: {
    name = "GenerateEncoder";
    system = builtins.currentSystem;
    packages.aarch64-linux.default = nixpkgs.legacyPackages.aarch64-linux.stdenv.mkDerivation {
      name = "GenerateEncoder";
      src = ./.;
      buildPhase = "make";
      installPhase = ''
        mkdir -p $out/bin
        cp output/bin/GenerateEncoder $out/bin/
      '';
    };
  };
}
