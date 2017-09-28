import i3s

public class Element {
  var e: OpaquePointer

  init() {
    e = Element_new()
  }
  deinit {
    Element_delete(e)
  }
}

public class FingerPrint {
  var f: OpaquePointer

  private static func do_init(ref: [Double], data: [Double], nr: Int) -> OpaquePointer {
    let ref6 = ref[0 ..< 6]
    let data_nr8 = data[0 ..< nr*8]
    
    return ref6.withUnsafeBufferPointer { ref6_unsafe in
      return data_nr8.withUnsafeBufferPointer { data_nr8_unsafe in
        return FingerPrint_new(ref6_unsafe.baseAddress, data_nr8_unsafe.baseAddress, Int32(nr))
      }
    }
  }

  public init(ref: [Double], data: [Double], nr: Int) {
    f = FingerPrint.do_init(ref: ref, data: data, nr: nr)
  } 
  deinit {
    FingerPrint_delete(f)
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
