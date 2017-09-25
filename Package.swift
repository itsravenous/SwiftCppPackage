// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "i3s",
    products: [
      .executable(name: "i3s", targets: ["i3s"]),
      .library(name: "SwiftExec", targets: ["swift-exec"]),
    ],
    targets: [
      .target(name: "sharkcpp"),
      .target(name: "sharkcwrapper", dependencies:["sharkcpp"]),
      .target(name: "swift-exec", dependencies:["sharkcwrapper"]),

      .target(name: "i3s")
    ]
)
