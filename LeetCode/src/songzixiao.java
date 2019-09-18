import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author cong
 * @date 2019-07-29
 */
public class songzixiao {
    public static void main(String[] args) {
        System.out.println(judge("杭州 杭州 杭州 北京 北京","aaab"));
    }

    public static boolean judge(String str, String pattern){
        String[] strs = str.split(" ");
        Map<Character, String> map = new HashMap<>();
        if(strs.length!=pattern.length()) return false;
        for(int i=0;i<pattern.length();i++){
            Character key = pattern.charAt(i);
            String value = strs[i];
            if(!map.containsKey(key)){
                map.put(key,value);
            }else{
                if(map.containsKey(key)&&!(map.get(key).equals(value))){
                    return false;
                }
            }
        }

        return true;

    }
}


