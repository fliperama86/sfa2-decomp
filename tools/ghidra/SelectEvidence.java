// Local only. All game-derived output stays ignored.
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.program.model.listing.Function;
import java.nio.file.*;
import java.util.*;
public class SelectEvidence extends GhidraScript {
 public void run() throws Exception {
  var args=getScriptArgs();var out=Path.of(args[0]);Files.createDirectories(out);var fs=new TreeMap<String,Function>();
  try(var w=Files.newBufferedWriter(out.resolve("references.tsv"))) {
   for(int i=1;i<args.length;i++) {
    var addr=toAddr(args[i]);var direct=getFunctionAt(addr);if(direct!=null)fs.put(addr.toString(),direct);
    for(var r:getReferencesTo(addr)){var f=getFunctionContaining(r.getFromAddress());w.write(addr+"\t"+r.getFromAddress()+"\t"+(f==null?"data":f.getEntryPoint()+" "+f.getName())+"\n");if(f!=null)fs.put(f.getEntryPoint().toString(),f);}
   }
  }
  var d=new DecompInterface();d.openProgram(currentProgram);
  try{for(var f:fs.values()){var r=d.decompileFunction(f,15,monitor);if(r.decompileCompleted())Files.writeString(out.resolve(f.getEntryPoint()+".c"),r.getDecompiledFunction().getC());}}finally{d.dispose();}
  println("SELECT_DONE functions="+fs.size());
 }
}
