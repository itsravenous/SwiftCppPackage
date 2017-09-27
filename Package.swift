// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "i3s",
    products: [
      .executable(name: "i3s", targets: ["I3S"]),
      .library(name: "SwiftExec", targets: ["swift-exec"]),
    ],
    targets: [
      .target(name: "sharkcpp"),
      .target(name: "sharkcwrapper", dependencies:["sharkcpp"]),
      .target(name: "swift-exec", dependencies:["sharkcwrapper"]),

      .target(name: "ElementC", publicHeadersPath: "incl"),
      .target(name: "ElementCWrapper", dependencies: ["ElementC"], publicHeadersPath: "incl"),
      .target(name: "Element", dependencies: ["ElementCWrapper"]),

      .target(name: "I3S", dependencies: ["Element"]),
    ]
)
