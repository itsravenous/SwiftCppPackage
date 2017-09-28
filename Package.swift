// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "i3s",
    products: [
      .executable(name: "i3s-c", targets: ["i3s-c"]),
      .library(name: "i3s-swift", targets: ["i3s-swift"]),
      .executable(name: "i3s-swift-exec", targets: ["i3s-swift-exec"]),
    ],
    targets: [
      .target(name: "i3s", publicHeadersPath: "incl_pub"),
      .target(name: "i3s-c", dependencies: ["i3s"]),
      .target(name: "i3s-swift", dependencies: ["i3s"]),
      .target(name: "i3s-swift-exec", dependencies: ["i3s-swift"]),
    ]
)
