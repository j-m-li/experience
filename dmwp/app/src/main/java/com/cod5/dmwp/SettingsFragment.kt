/*
 *          PUBLIC DOMAIN 20 August MMXXIII by O'ksi'D
 *
 *        The authors disclaim copyright to this software.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a
 * compiled binary, for any purpose, commercial or non-commercial,
 * and by any means.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT OF ANY PATENT, COPYRIGHT, TRADE SECRET OR OTHER
 * PROPRIETARY RIGHT.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

package com.cod5.dmwp

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import androidx.core.view.forEach
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.fragment.app.Fragment
import com.cod5.dmwp.databinding.FragmentSettingsBinding
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.runBlocking

class SettingsFragment : Fragment() {

    private var _binding: FragmentSettingsBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentSettingsBinding.inflate(inflater, container, false)
        binding.root.isSaveEnabled = true
        return binding.root
    }

    override fun onStart() {
        super.onStart()
        MainActivity.settingsVisible = true
    }

    override fun onPause() {
        super.onPause()
        MainActivity.settingsVisible = false
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        restore()
        /*
        binding.buttonSecond.setOnClickListener {
            findNavController().navigate(R.id.action_SecondFragment_to_FirstFragment)
        }*/
    }

    private fun restore() {
        val pref = MainActivity.getPref()
        binding.root.forEach {
            if (it is ViewGroup) {
                it.forEach {
                    if (it is EditText) {
                        val v: EditText = it
                        val xx: Flow<String> = pref.data.map { preferences ->
                            preferences[stringPreferencesKey(v.id.toString())] ?: v.text.toString()
                        }
                        runBlocking { v.setText(xx.first()) }
                    }
                }
            }
        }
    }

    private suspend fun save() {
        val pref = MainActivity.getPref()
        binding.root.forEach {
            if (it is ViewGroup) {
                it.forEach {
                    if (it is EditText) {
                        val v: EditText = it
                        pref.edit { preferences ->
                            preferences[stringPreferencesKey(v.id.toString())] = v.text.toString()
                        }
                    }
                }
            }
        }
    }

    override fun onStop() {
        runBlocking { save() }
        super.onStop()
    }

    override fun onDestroyView() {
        super.onDestroyView()

        _binding = null
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
    }
}