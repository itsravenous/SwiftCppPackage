import i3s

public class Element {
  var e: OpaquePointer

  init() {
    e = Element_new()
  }
  deinit {
    // TODO Element_delete(e)
  }
}

public class FingerPrint {
  var f: OpaquePointer

  public init(ref: inout [Double], data: inout [Double], nr: Int) {
    f = FingerPrint_new(&ref, &data, Int32(nr));
  } 
  deinit {
    // TODO FingerPrint_delete(f)
  }

  public func compare(_ unknown: FingerPrint) -> Double {
    return Compare_two(unknown.f, f)
  }
  
  // Will you actually want this as a separate property?
  // The score is already returned by compare()
  public var comparisonScore: Double {
    get {
      return FingerPrint_getScore(f)
    }
  }
}
