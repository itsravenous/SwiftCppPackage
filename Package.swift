import PackageDescription

let package = Package(
    name: "i3s",
    targets: [
      Target(name: "sharkcwrapper", dependencies:["sharkcpp"]),
      Target(name: "swift-exec", dependencies:["sharkcwrapper"]),
    ]
)
