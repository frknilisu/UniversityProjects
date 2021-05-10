import org.json.JSONArray;
import org.json.JSONException;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;

public class Helper {

    //private static Map<String, Integer> filesMap = new HashMap<String, Integer>();
    private static ReaderWriter[] fileSemaphores;
    private static int numberOfFiles = 0;
    public static ArrayList<String> filesName = new ArrayList<>();
    public static JSONArray fileJson = new JSONArray();

    public Helper() {
    }

    public static void initJson() throws IOException, JSONException {

        File initFile = new File("./db/init.json");

        FileInputStream fin = new FileInputStream(initFile);

        int length = fin.available();
        byte[] buff = new byte[length];
        int bytesread = 0;

        while ((bytesread = fin.read(buff)) != -1) {
            // convert to char and display it
        }
        fin.close();

        String content = new String(buff);
        fileJson = new JSONArray(content);
        System.out.println(fileJson.toString());

    }

    private static void fillNameList() throws JSONException {
        filesName.clear();
        numberOfFiles = fileJson.length();
        for (int i = 0; i < numberOfFiles; i++)
            filesName.add(fileJson.getJSONObject(i).getString("name"));
    }


    private static void initSemaphores() {
        fileSemaphores = new ReaderWriter[numberOfFiles];
        for (int i = 0; i < numberOfFiles; i++)
            fileSemaphores[i] = new ReaderWriter();
    }

    public static void init() throws IOException, JSONException {
        initJson();
        fillNameList();
        initSemaphores();
    }

    public static ReaderWriter getSemaphoreOfTheFile(int fileId) {
        return fileSemaphores[fileId];
    }

}
