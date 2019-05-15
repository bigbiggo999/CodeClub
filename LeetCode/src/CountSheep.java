import java.util.HashMap;
import java.util.Map;

/**
 * @author cong
 * @date 2019-05-14
 */
public class CountSheep {
    public static void main(String[] args) {
        //第几年之后的羊?
        getSheep(12);
    }


    public static Integer getSheep(int n){
        if(n<=0) {
            return 0;
        }
        Map<Integer,Integer> all = new HashMap<>();
        Map<Integer,Integer> map = new HashMap<>();
        Map<Integer,Integer> dead = new HashMap<>();
        all.put(1,0);
        int year = 1;
        int maxOrder = 1;
        while (year<=n){
            for (Map.Entry<Integer, Integer> entry : all.entrySet()) {
                Integer order = entry.getKey();
                Integer age = entry.getValue();
                age++;
                if (age <=2) {
                    all.put(order,age);
                } else if (age >= 3 && age <= 7) {
                    all.put(order,age);
                    map.put(++maxOrder, 1);
                    System.out.println("今年是第"+year+"年, "+"编号为"+order+",年龄为"+age+"的羊,"+"新生编号为"+maxOrder+"的羊");
                }else if(age>7 && age<=9){
                    all.put(order,age);
                }
                else if (age==10){
                    System.out.println("今年是第"+year+"年,"+"编号为"+order+"的羊将去世,年龄为 "+age);
                    dead.put(order,age);
                }
            }
            all.putAll(map);
            map.clear();
            dead.forEach((order,age)->all.remove(order));
            dead.clear();
            year++;
        }
        System.out.println("总共有"+all.size()+"只羊");
        System.out.println(all.toString());
        return 0;
    }
}
