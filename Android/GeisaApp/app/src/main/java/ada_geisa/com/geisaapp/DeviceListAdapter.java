package ada_geisa.com.geisaapp;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.lang.reflect.Array;
import java.util.ArrayList;

/**
 * Created by henri on 20/07/2017.
 */

public class DeviceListAdapter extends ArrayAdapter<BluetoothDevice> {

    private LayoutInflater mLayoutInflater;
    private ArrayList<BluetoothDevice> mDevices;
    private int mViewResourceID;

    public DeviceListAdapter(Context context, int tvResourceID, ArrayList<BluetoothDevice> devices){
        super(context, tvResourceID, devices);
        this.mDevices = devices;
        mLayoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        mViewResourceID = tvResourceID;
    }

    public View getView(int position, View convertView, ViewGroup parent){
        convertView = mLayoutInflater.inflate(mViewResourceID, null);

        BluetoothDevice device = mDevices.get(position);

        if(device != null){
            TextView deviceName = (TextView) convertView.findViewById(R.id.tvDeviceName);
            TextView deviceAddress = (TextView) convertView.findViewById(R.id.tvDeviceAddress);

            if(deviceName != null){
                deviceName.setText(device.getName());
            }

            if(deviceAddress != null){
                deviceAddress.setText(device.getAddress());
            }
        }
        return convertView;
    }
}
