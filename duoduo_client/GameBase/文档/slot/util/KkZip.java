package com.xunbao.slot.util;

public class KkZip {

	public static java.util.List<java.io.File> GetFileList(String zipFileString, boolean bContainFolder, boolean bContainFile)throws Exception {  
        
        android.util.Log.v("kkZip", "GetFileList(String)");  
          
        java.util.List<java.io.File> fileList = new java.util.ArrayList<java.io.File>();  
        java.util.zip.ZipInputStream inZip = new java.util.zip.ZipInputStream(new java.io.FileInputStream(zipFileString));  
        java.util.zip.ZipEntry zipEntry;  
        String szName = "";  
          
        while ((zipEntry = inZip.getNextEntry()) != null) {  
            szName = zipEntry.getName();  
          
            if (zipEntry.isDirectory()) {  
          
                // get the folder name of the widget  
                szName = szName.substring(0, szName.length() - 1);  
                java.io.File folder = new java.io.File(szName);  
                if (bContainFolder) {  
                    fileList.add(folder);  
                }  
          
            } else {  
                java.io.File file = new java.io.File(szName);  
                if (bContainFile) {  
                    fileList.add(file);  
                }  
            }  
        }//end of while  
          
        inZip.close();  
          
        return fileList;  
    }  
  
    /** 
     * ����ѹ�����е��ļ�InputStream 
     * @param zipFileString     ѹ���ļ������� 
     * @param fileString    ��ѹ�ļ������� 
     * @return InputStream 
     * @throws Exception 
     */  
    public static java.io.InputStream UpZip(String zipFileString, String fileString)throws Exception {  
        android.util.Log.v("kkZip", "UpZip(String, String)");  
        java.util.zip.ZipFile zipFile = new java.util.zip.ZipFile(zipFileString);  
        java.util.zip.ZipEntry zipEntry = zipFile.getEntry(fileString);  
          
        return zipFile.getInputStream(zipEntry);  
  
    }  
      
      
    /** 
     * ��ѹһ��ѹ���ĵ� ��ָ��λ�� 
     * @param zipFileString ѹ��������� 
     * @param outPathString ָ����·�� 
     * @throws Exception 
     */  
    public static void UnZipFolder(String zipFileString, String outPathString)throws Exception {  
        android.util.Log.v("kkZip", "UnZipFolder(String, String)");  
        java.util.zip.ZipInputStream inZip = new java.util.zip.ZipInputStream(new java.io.FileInputStream(zipFileString));  
        java.util.zip.ZipEntry zipEntry;  
        String szName = "";  
          
        while ((zipEntry = inZip.getNextEntry()) != null) {  
            szName = zipEntry.getName();  
          
            if (zipEntry.isDirectory()) {  
          
                // get the folder name of the widget  
                szName = szName.substring(0, szName.length() - 1);  
                java.io.File folder = new java.io.File(outPathString + java.io.File.separator + szName);  
                folder.mkdirs();  
          
            } else {  
          
                java.io.File file = new java.io.File(outPathString + java.io.File.separator + szName);  
                file.createNewFile();  
                // get the output stream of the file  
                java.io.FileOutputStream out = new java.io.FileOutputStream(file);  
                int len;  
                byte[] buffer = new byte[1024];  
                // read (len) bytes into buffer  
                while ((len = inZip.read(buffer)) != -1) {  
                    // write (len) byte from buffer at the position 0  
                    out.write(buffer, 0, len);  
                    out.flush();  
                }  
                out.close();  
            }  
        }//end of while  
          
        inZip.close();  
      
    }//end of func  
      
  
    /** 
     * ѹ���ļ�,�ļ��� 
     * @param srcFileString Ҫѹ�����ļ�/�ļ������� 
     * @param zipFileString ָ��ѹ����Ŀ�ĺ����� 
     * @throws Exception 
     */  
    public static void ZipFolder(String srcFileString, String zipFileString)throws Exception {  
        android.util.Log.v("kkZip", "ZipFolder(String, String)");  
          
        //����Zip��  
        java.util.zip.ZipOutputStream outZip = new java.util.zip.ZipOutputStream(new java.io.FileOutputStream(zipFileString));  
          
        //��Ҫ������ļ�  
        java.io.File file = new java.io.File(srcFileString);  
  
        //ѹ��  
        ZipFiles(file.getParent()+java.io.File.separator, file.getName(), outZip);  
          
        //���,�ر�  
        outZip.finish();  
        outZip.close();  
      
    }//end of func  
      
    /** 
     * ѹ���ļ� 
     * @param folderString 
     * @param fileString 
     * @param zipOutputSteam 
     * @throws Exception 
     */  
    private static void ZipFiles(String folderString, String fileString, java.util.zip.ZipOutputStream zipOutputSteam)throws Exception{  
        android.util.Log.v("kkZip", "ZipFiles(String, String, ZipOutputStream)");  
          
        if(zipOutputSteam == null)  
            return;  
          
        java.io.File file = new java.io.File(folderString+fileString);  
          
        //�ж��ǲ����ļ�  
        if (file.isFile()) {  
  
            java.util.zip.ZipEntry zipEntry =  new java.util.zip.ZipEntry(fileString);  
            java.io.FileInputStream inputStream = new java.io.FileInputStream(file);  
            zipOutputSteam.putNextEntry(zipEntry);  
              
            int len;  
            byte[] buffer = new byte[4096];  
              
            while((len=inputStream.read(buffer)) != -1)  
            {  
                zipOutputSteam.write(buffer, 0, len);  
            }  
              
            zipOutputSteam.closeEntry();  
        }  
        else {  
              
            //�ļ��еķ�ʽ,��ȡ�ļ����µ����ļ�  
            String fileList[] = file.list();  
              
            //���û�����ļ�, ����ӽ�ȥ����  
            if (fileList.length <= 0) {  
                java.util.zip.ZipEntry zipEntry =  new java.util.zip.ZipEntry(fileString+java.io.File.separator);  
                zipOutputSteam.putNextEntry(zipEntry);  
                zipOutputSteam.closeEntry();                  
            }  
              
            //��������ļ�, �������ļ�  
            for (int i = 0; i < fileList.length; i++) {  
                ZipFiles(folderString, fileString+java.io.File.separator+fileList[i], zipOutputSteam);  
            }//end of for  
      
        }//end of if  
          
    }//end of func  
      
    public void finalize() throws Throwable {  
          
    }  
}
