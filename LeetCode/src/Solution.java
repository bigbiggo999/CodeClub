/**
 * @author cong
 * @date 2019-03-12
 */
public class Solution {
    /*
    2 3 4
    5 6 7
    1 2
    9 9
     */
    public ListNode addTwoNumbers(ListNode l1, ListNode l2) {
        int sum = 0;
        if (l1 == null || l2 == null ){
            ListNode result;
            result = (l1 == null)?l2:l1;
            return result;
        }
        int num1=0,num2=0,num=0,plus=0,willplus = 0;
        ListNode result = null;
        ListNode nowNode = new ListNode(0);
        while (true){
            num = l1.val+l2.val + plus;
            if (num>=10) {
                num = num %10;
                willplus = 1;
            }
            plus = willplus;
            willplus = 0;

            if(result == null){
                result = new ListNode(num);
                nowNode = result;
            }else{
                ListNode newNode = new ListNode(num);
                nowNode.next = newNode;
                nowNode = newNode;
            }
            if (l1.next == null && l2.next == null){
                if (plus ==1){
                    ListNode last = new ListNode(1);
                    nowNode.next = last;
                }
                break;
            }
            if (l1.next != null){
                l1 = l1.next;
            }else {
                l1.val = 0;
            }
            if (l2.next != null){
                l2 = l2.next;
            }else {
                l2.val = 0;
            }
        }


        return result;


    }


    public static int lengthOfLongestSubstring(String s) {
        /*
        abcbadef
         */
        int max = 0,beginIndex = 0, length = 0;
        StringBuilder str = new StringBuilder("");
        for(int i = 0; i < s.length(); i++){
            if(!str.toString().contains(s.substring(i,i+1))){
                str.append(s.charAt(i));
                length++;
            }else{
                int repeatIndex = str.indexOf(String.valueOf(s.charAt(i)));
                str = new StringBuilder(str.substring(repeatIndex+1));
                str.append(s.charAt(i));
                if (length>max){
                    max = length;
                }
                length = str.length();
            }
        }
        if (length>max) max = length;
        return max;
    }

    public static void main(String[] args) {
//        Integer[] a = new Integer[]{1,2,3};
        String s = "ee";
        System.out.println(lengthOfLongestSubstring(s));
    }
}

class ListNode {
    int val;
    ListNode next;

    ListNode(int x) {
        val = x;
    }
}
