import FingerPrintCWrapper

func pointerDoubleArray(p: UnsafeMutablePointer<UnsafeMutablePointer<Double>>) -> UnsafeMutablePointer<UnsafeMutablePointer<Double>> {
  return p
}

func pointerInt(p: UnsafeMutablePointer<Int>) -> UnsafeMutablePointer<Int> {
  return p
}

public func createFingerPrint(refs: [Double], keyPoints: [Double]) -> OpaquePointer {
  let refsPointer = pointerDoubleArray(p: &refs)
  let keyPoints = pointerDoubleArray(p: &keyPoints)
  let count = keyPoints.count / 2
  let countPointer = pointerInt(p: &count)
  return FingerPrint_new(refs, keyPoints, countPointer)
}
