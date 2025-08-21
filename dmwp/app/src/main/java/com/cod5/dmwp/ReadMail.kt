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

import android.util.Log
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import com.sun.mail.imap.IMAPFolder
import java.util.*
import javax.mail.*


class ReadMail(
    private val host: String,
    private val port: String,
    private val user: String,
    private val password: String,
    private val qu: LinkedList<Pair<String, String>>
) {

    companion object {
        public var session: Session? = null
    }

    fun execute() {
        //Log.d("readmail", host)
        val props = Properties()
        props["mail.store.protocol"] = "imaps";
        props["mail.imap.host"] = host
        props["mail.imap.socketFactory.port"] = port
        props["mail.imap.socketFactory.fallback"] = "false"
        props["mail.imap.socketFactory.class"] = "javax.net.ssl.SSLSocketFactory"
        props["mail.imap.port"] = port
        if (session == null) {
            session = Session.getInstance(props)
        }
        try {
            // connects to the message store
            val store = session!!.getStore("imap")
            store.connect(user, password)

            // opens the inbox folder
            val folderInbox = store.getFolder("INBOX")
            folderInbox.open(Folder.READ_WRITE)

            // fetches new messages from server
            val messages = folderInbox.messages

            // process and tag email
            for (me in messages) {
                qu.add(Pair(me.from.first().toString(), me.subject.toString()))
                me.setFlag(Flags.Flag.DELETED, true)
            }
            folderInbox.expunge()
            // disconnect
            folderInbox.close(false)
            store.close()
        } catch (e: NoSuchProviderException) {
            MainFragment.log = "No provider for protocol: imap"
        } catch (e: MessagingException) {
            MainFragment.log = "Could not connect to the message store"
        }
    }
}