import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;


public class GetFiveModelsByParsingQprobAndMUFOLD {
	
	
	public static void main(String[] args) throws IOException {
		
//		if(args.length < 2){
//			
//			System.out.println("Please provide the protein name as argument");
//			
//		}
		
		String prot_id = args[0];
		String file_extension = args[1];
//		System.out.println("");
//		String prot_id = "R0996-D4";
		
		
		File currentDir = new File(new File(".").getAbsolutePath());
		String curr_dir_path = currentDir.getCanonicalPath();
		
				
		String qprob_file = curr_dir_path+"/"+prot_id+".Qprob_score";
//		System.out.println(qprob_file);
		String cluster_file = curr_dir_path+"/"+prot_id+".log";
//		System.out.println(cluster_file);
		
		String op_unicon_five_best_models_file = curr_dir_path+"/"+prot_id+"."+file_extension;
		
		parseAndPrintFiveModelsName(qprob_file, cluster_file, op_unicon_five_best_models_file);
		
		
		
	}
	
	public static void parseAndPrintFiveModelsName(String qprob_file, String cluster_file, String op_unicon_five_best_models_file) throws IOException{
		
		BufferedReader qprob_rd = BufferReaderAndWriter.getReader(new File(qprob_file));
		BufferedReader cluster_rd = BufferReaderAndWriter.getReader(new File(cluster_file));
		BufferedWriter unicon_best_models_wr = BufferReaderAndWriter.getWriter(new File(op_unicon_five_best_models_file));
		String qprob_line;
		String cluster_line;
		
		Map<String, Integer> cluster_map = new HashMap<String, Integer>();
		Map<Integer, String> models_map = new HashMap<Integer, String>();
//		ArrayList<String> cluster_file_name = new ArrayList<String>();
//		ArrayList<Integer> cluster_id = new ArrayList<Integer>();
		
		while((cluster_line = cluster_rd.readLine())!=null){
			
			if(!cluster_line.equalsIgnoreCase("INFO  :    Item     Cluster                     DecoyName  ")){
				
				continue;
				
			}else{
				
				while((cluster_line = cluster_rd.readLine()) != null){
					
					if((cluster_line.equalsIgnoreCase("INFO  : ======================================")) || (cluster_line.equalsIgnoreCase("INFO  :    Item     Cluster                     DecoyName  "))){
												
						continue;
						
					}else{
						
						String[] cluster_tokens = cluster_line.split("\\s+");
						
						if(cluster_map.containsKey(cluster_tokens[5])){ // if the decoyName already exist in the map then continue
							
							continue;
						}
						
//						if(cluster_file_name.contains(cluster_tokens[5])){
//							
//							continue;
//							
//						}
						
						cluster_map.put(cluster_tokens[5], Integer.parseInt(cluster_tokens[4])); // add the decoyName as a key and cluster index as a value
//						cluster_file_name.add(cluster_tokens[5]);
//						cluster_id.add(Integer.parseInt(cluster_tokens[4]));
//						System.out.println(cluster_tokens[5]+"\t"+Integer.parseInt(cluster_tokens[4]));
												
						
					}
					
				}
							
				
				
			}
			
			
			
		}
		
		cluster_rd.close();
		
		
		// the best 5 models representing 5 different clusters is selected based on the Qprob score 
		while((qprob_line = qprob_rd.readLine())!=null){
			
			String[] qprob_token = qprob_line.split("\t");
			String fileName = qprob_token[0];
			
			if(models_map.size() == 5){
				
				break;
				
			}
			
//			if(cluster_file_name.contains(fileName) && models_map.containsKey(cluster_id.get(cluster_file_name.indexOf(fileName)))){
//				
//				continue;
//				
//			}else if(cluster_file_name.contains(fileName) && !models_map.containsKey(cluster_id.get(cluster_file_name.indexOf(fileName)))){
//				
//				models_map.put(cluster_id.get(cluster_file_name.indexOf(fileName)), fileName);
//				
//			}else{
//				
//				continue;
//				
//			}
			
			
			if(cluster_map.containsKey(fileName) && models_map.containsKey(cluster_map.get(fileName))){
				
				continue;
				
			}else if(cluster_map.containsKey(fileName) && !models_map.containsKey(cluster_map.get(fileName))){
				
				models_map.put(cluster_map.get(fileName), fileName);
				
			}else{
				
				continue;
				
			}
			
			
		}
		
		qprob_rd.close();
		
		
		for(int key : models_map.keySet()){
			
//			System.out.print(key+"\t");
//			System.out.println(models_map.get(key));
			unicon_best_models_wr.write(models_map.get(key));
			unicon_best_models_wr.newLine();
			
			
		}
		
		unicon_best_models_wr.close();
		
		
	}
	

}
