// Local analysis only. Output can contain proprietary game-derived code.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.*;
import java.util.*;
public class AuditProgram extends GhidraScript {
 public void run() throws Exception {
  String[] a=getScriptArgs();Path out=Path.of(a[0]);Files.createDirectories(out);
  for(int i=1;i<a.length;i++) {var addr=toAddr(a[i]);disassemble(addr);createFunction(addr,null);}
  analyzeAll(currentProgram);
  try(var w=Files.newBufferedWriter(out.resolve("instructions.txt"))) {
   for(var it=currentProgram.getListing().getInstructions(true);it.hasNext();){var in=it.next();w.write(in.getAddress()+" "+in+"\n");}
  }
  try(var w=Files.newBufferedWriter(out.resolve("functions.tsv"))) {
   for(var it=currentProgram.getFunctionManager().getFunctions(true);it.hasNext();){var f=it.next();w.write(f.getEntryPoint()+"\t"+f.getName()+"\t"+f.getBody().getNumAddresses()+"\t"+f.getSignature()+"\n");}
  }
  try(var w=Files.newBufferedWriter(out.resolve("symbols.tsv"))) {
   for(var it=currentProgram.getSymbolTable().getAllSymbols(true);it.hasNext();){var s=it.next();if(s.getSource()!=SourceType.DEFAULT)w.write(s.getAddress()+"\t"+s.getName()+"\t"+s.getSource()+"\n");}
  }
  var dec=new DecompInterface();dec.openProgram(currentProgram);int n=0;
  try {
   for(var it=currentProgram.getFunctionManager().getFunctions(true);it.hasNext()&&n<40;){var f=it.next();if(f.getBody().getNumAddresses()<24)continue;var r=dec.decompileFunction(f,15,monitor);if(r.decompileCompleted()&&r.getDecompiledFunction()!=null){Files.writeString(out.resolve(f.getEntryPoint()+".c"),r.getDecompiledFunction().getC());n++;}}
  } finally {dec.dispose();}
  println("AUDIT_DONE functions="+currentProgram.getFunctionManager().getFunctionCount()+" decompiled="+n);
 }
}
