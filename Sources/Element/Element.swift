import ElementCWrapper

public func createElement() -> OpaquePointer {
  return Element_new()
}
